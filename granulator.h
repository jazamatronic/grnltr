#pragma once

#include "grain.h"
#include "crc_noise.h"

#include "params.h"

#define MAX_GRAINS 16
// Let's stick to 16bit samples for now
// This can be templated later
class Granulator
{
  public:
    Granulator() {}
    ~Granulator() {}

    void Init(float sr, int16_t *start, size_t len, float *env, size_t env_len) 
    {
      sr_ = sr;
      sample_start_ = start;
      len_ = len;
      env_mem_ = env;
      env_len_ = env_len;
      Setup();
      live_ = filled_ = false;
      rng.Init();
    }

    void Stop()
    {
      stop_ = true;
      filled_ = false;
    }

    void Start()
    {
      stop_ = false;
    }

    void ReStart()
    {
      sample_pos_.Reset();
      stop_ = false;
    }

    void Reset(int16_t *start, size_t len) 
    {
      sample_start_ = start;
      len_ = len;
      Setup();
      live_ = filled_ = false;
    }

    /*
     * This live looping is very naive, shoehorning the record buffer into a Phasor/SamplePhasor
     * instead of something specifically designed for it.
     * A proper record buffer should be in something like a delay line or circular buffer 
     * where the start and end points are moved and wrapped accordingly as the write head moves.
     * Maybe its possible to retrofit that into the existing grain code with a little more thought and effort
     */
    void Live(int16_t *start, size_t len) 
    {
      sample_start_ = start;
      len_ = len;
      record_buf_ = sample_start_;
      write_pos_ = 0;
      Setup();
      live_ = true;
      filled_ = false;
    }

    // don't allow this in live mode
    void ToggleSampleLoop()
    {
      if (!live_) {
	sample_loop_ = sample_loop_ ? false : true;
      	sample_pos_.ToggleLoop();
      	if (sample_loop_) Start();
      }
    }

    void SetGrainDuration(float dur)
    {
      grain_dur_ = dur;
    }

    void SetGrainPitch(float pitch)
    {
      grain_pitch_ = pitch;
    }

    void SetPitchDist(float dist)
    {
      pitch_dist_ = dist;
    }

    // don't allow this in live mode
    void SetScanRate(float rate)
    {
      if (!live_) {
	sample_pos_.SetPitch(rate);
      }
    }

    // dist is % of the sample length
    // scatter_dist_ is stored as number of samples
    void SetScatterDist(float dist)
    {
      scatter_dist_ = dist * len_;
    }

    void ChangeEnv(float *env)
    {
      env_mem_ = env;
    }

    void Dispatch(size_t sample_pos)
    {
      float rand;
      float pitch = grain_pitch_;
      for (size_t i = 0; i < MAX_GRAINS; i++) {
	if (silo[i].IsDone()) {
	  if (random_pitch_) {
	    rand = rng.Process();
	    pitch = fminf(4.0f, fmaxf(0.25f, pitch * (1.0 + (rand * pitch_dist_))));
	  }
	  silo[i].Dispatch(sample_pos, grain_dur_, env_mem_, pitch, reverse_grain_);
	  return;
	}
      }
    }

    // density is number of samples until a new grain is dispatched
    void SetDensity(int32_t density)
    {
      density_ = density;
    }


    // don't allow this in live mode
    void ToggleScanReverse()
    {
      if (!live_) {
	sample_pos_.ToggleReverse();
      }
    }

    void ToggleGrainReverse()
    {
      reverse_grain_ = reverse_grain_ ? false : true;
    }

    void ToggleScatter()
    {
      scatter_grain_ = scatter_grain_ ? false : true;
    }

    void ToggleFreeze()
    {
      freeze_ = freeze_ ? false : true;
      scatter_grain_ = freeze_;
    }

    void ToggleRandomPitch()
    {
      random_pitch_ = random_pitch_ ? false : true;
    }

    void ToggleRandomDensity()
    {
      random_density_ = random_density_ ? false : true;
    }

    // All pos are in the range 0 to 1
    // They will be multiplied by len_ to get actual size_t sample position

    // don't allow this in live mode
    void SetSampleStart(float pos)
    {
      if (!live_) {
	sample_pos_.SetStartPos(pos);
      }
    }

    // don't allow this in live mode
    void SetSampleEnd(float pos)
    {
      if (!live_) {
	sample_pos_.SetEndPos(pos);
      }
    }


    float Process(int16_t input)
    {
      float sample = 0;
      float rand;
      int32_t offset;
      size_t pos = 0;
      bool eot = false;

      if (stop_) return 0.0f;

      if (freeze_)
      {
	pos = sample_pos_.GetPos();
      } else {
	if (live_) { record_buf_[write_pos_] = input; }
	pos = sample_pos_.Process(&eot);
	write_pos_ = pos;
	if (eot) { filled_ = true; }
      }

      if (density_count_-- < 0)
      {
	if (random_density_) {
	  rand = rng.Process();
	  density_count_ = rand * density_;
	} else {
	  density_count_ = density_;
	}

	if (scatter_grain_) {
	  rand = rng.Process();
	  offset = rand * scatter_dist_;


// Do the pragma dance - we take care of wrap around 
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
	  offset += pos;
	  if (offset < 0) {
	    offset += len_;
	  } else if (offset > len_) {
	    offset -= len_;
	  }
#pragma GCC diagnostic pop
	  pos = (size_t)offset;
	}
	if (!live_ || filled_) { Dispatch(pos); }
      }

      for (size_t i = 0; i < MAX_GRAINS; i++) {
	if (!silo[i].IsDone()) {
	  sample += silo[i].Process();
	}
      }

      if (eot && (sample_loop_ == false)) Stop();
      // clamp?
      return fminf(1.0f, fmaxf(-1.0f, sample));
    }

  private:

    void Setup()
    {
      sample_pos_.Init(sr_, len_);
      sample_pos_.ToggleLoop();
      sample_loop_ = true;
      density_count_ = -1;
      SetGrainDuration(DEFAULT_GRAIN_DUR);
      SetGrainPitch(DEFAULT_GRAIN_PITCH);
      SetScanRate(DEFAULT_SCAN_RATE);
      SetDensity(sr_/DEFAULT_GRAIN_DENS);
      SetScatterDist(DEFAULT_SCATTER_DIST);
      SetPitchDist(DEFAULT_PITCH_DIST);
      stop_ = random_pitch_ = scatter_grain_ = reverse_grain_ = random_density_ = false;
      for (size_t i = 0; i < MAX_GRAINS; i++) {
	silo[i].Init(sr_, sample_start_, len_, DEFAULT_GRAIN_VOL, env_mem_, env_len_);
      }
    }

    Grain<int16_t> silo[MAX_GRAINS];
    Phasor sample_pos_;
    int16_t *sample_start_;  
    size_t len_, env_len_, scatter_dist_, write_pos_;
    int32_t density_, density_count_;
    float sr_, grain_dur_, grain_pitch_, pitch_dist_;
    float *env_mem_;
    bool sample_loop_, stop_, reverse_grain_, scatter_grain_, random_pitch_, random_density_, freeze_;
    daisysp::crc_noise rng;

    // used for live record buffer only
    int16_t *record_buf_;  
    bool live_, filled_;
};

