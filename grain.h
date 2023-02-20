#pragma once

#include "sample_phasor.h"

template <typename T>
class Grain
{
  public:
    Grain() {}
    ~Grain() {} 

    void Init(float sr, T *start, size_t len, float vol, float *env, size_t env_len) 
    {
      sample_.Init(start, sr, len);
      env_.Init(env, sr, env_len); 
      vol_ = vol;
      done_ = true;
    }

    // pitch usually in the range of 0.5 to 2.0 (-8va to +8va)
    // 1 is no pitch adjustment
    void SetSamplePitch(float pitch)
    {
      sample_.SetPitch(pitch);
    }

    // set the grain duration in s
    // this is how long it will take to scan the envelope of length env_len_ pointed to by env_
    void SetGrainDuration(float dur)
    {
      env_.SetDur(dur);
    }

    void SetSamplePos(size_t pos) 
    {
      sample_.SetCurPos(pos);
    }

    inline bool IsDone()
    {
      return done_;
    }

    void StartAt(size_t sample_pos)
    {
      sample_.Reset();
      sample_.SetCurPos(sample_pos);
      env_.Reset();
      done_ = false;
    }

    void Reset()
    {
      sample_.Reset();
      env_.Reset();
      done_ = false;
    }

    void ResetEnv()
    {
      env_.Reset();
      done_ = false;
    }

    void SetReverse(bool r)
    {
      sample_.SetReverse(r);
    }

    void ChangeEnv(float *env)
    {
      env_.SetSample(env);
    }

    void SetGrainVol(float vol)
    {
      vol_ = vol;
    }


    void Dispatch(size_t sample_pos, float dur, float *env, float pitch, bool r)
    {
      sample_.Reset();
      sample_.SetCurPos(sample_pos);
      sample_.SetPitch(pitch);
      sample_.SetReverse(r);

      env_.Reset();
      env_.SetDur(dur);
      env_.SetSample(env);

      done_ = false;
    }

    float Process()
    {
      float env, sample;
      bool sample_done;

      if (done_) {
	sample = 0.0f;
      } else {
      	env = env_.Process(&done_);
	sample = sample_.Process(&sample_done) * vol_ * env;
      }
      return sample;
    }

  private:
    Sample<T> sample_;
    Sample<float> env_;
    float vol_;
    bool done_;
};
