#include "pod.h"
#include "led_colours.h"

DaisyPod hw;
static Parameter knob1, knob2;

void UpdateEncoder()
{
  cur_page = (int8_t)(cur_page + hw.encoder.Increment());
  if (cur_page >= NUM_PAGES) { cur_page = 0; }
  if (cur_page < 0) { cur_page += NUM_PAGES; }
  switch(cur_page)
  {
    case 0:
      hw.led1.Set(RED);
      /*
       * k1 = Grain Pitch
       * k2 = Scan Rate (disabled in live mode)
       * b1 = cycle env type
       * b2 = Reset Grain Pitch and Scan Rate
       */
      break;
    case 1:
      hw.led1.Set(ORANGE);
      /*
       * k1 = Grain Duration
       * k2 = Grain Density
       * b1 = Grain Reverse
       * b2 = Scan Reverse (disabled in live mode)
       */
      break;
    case 2:
      hw.led1.Set(YELLOW);
      /*
       * k1 = Scatter Distance
       * b1 = Toggle Scatter
       * b2 = Toggle Freeze
       */
      break;
    case 3:
      hw.led1.Set(GREEN);
      /*
       * k1 = Pitch Distance
       * b1 = Toggle Random Pitch
       * b2 = Toggle Random Density
       */
      break;
    case 4:
      hw.led1.Set(BLUE);
      /*
       * NOTE: led flashes blue during start up to indicate file reading
       * k1 = Sample Start (disabled in live mode) 
       * k2 = Sample End (disabled in live mode)
       * b1 = Cycle Wave
       * b2 = Toggle Wave Loop (disabled in live mode)
       */
      break;
    case 5:
      hw.led1.Set(PURPLE);
      /*
       * k1 = Bit Crush
       * k2 = Downsample
       * b1 = Live Mode
       * b2 = Sample Mode
       */
      break;
    default:
      break;
  }
  hw.UpdateLeds();
}

void UpdateButtons()
{
  switch(cur_page)
  {
    case 0:
      if(hw.button1.RisingEdge()) {
	cur_grain_env++;
	if (cur_grain_env == NUM_GRAIN_ENVS) {
	  cur_grain_env = 0;
	}
	grnltr.ChangeEnv(grain_envs[cur_grain_env]);
      }
      if(hw.button2.RisingEdge()) {
	pitch_p.Lock(1.0);
  	rate_p.Lock(1.0);
	mmh.ResetGotClock();
      }
      break;
    case 1:
      if(hw.button1.RisingEdge()) {
	grnltr.ToggleGrainReverse();
      }
      if(hw.button2.RisingEdge()) {
	grnltr.ToggleScanReverse();
      }
      break;
    case 2:
      if(hw.button1.RisingEdge()) {
	grnltr.ToggleScatter();
      }
      if(hw.button2.RisingEdge()) {
	grnltr.ToggleFreeze();
      }
      break;
    case 3:
      if(hw.button1.RisingEdge()) {
	grnltr.ToggleRandomPitch();
      }
      if(hw.button2.RisingEdge()) {
	grnltr.ToggleRandomDensity();
      }
      break;
    case 4:
      if(hw.button1.RisingEdge()) {
	cur_wave++;
	if (cur_wave >= wav_file_count) cur_wave = 0;
	grnltr.Stop();
	InitControls();
	grnltr.Reset( \
	    &sm[wav_start_pos[cur_wave]], \
	    wav_file_names[cur_wave].raw_data.SubCHunk2Size / sizeof(int16_t));
    	grnltr.Dispatch(0);
      }
      if(hw.button2.RisingEdge()) {
	grnltr.ToggleSampleLoop();
      }
      break;
    case 5:
      if(hw.button1.RisingEdge()) {
	grnltr.Stop();
	InitControls();
	grnltr.Live( \
	    &sm[0], \
	    live_rec_buf_len);
      }
      if(hw.button2.RisingEdge()) {
	grnltr.Stop();
	InitControls();

	grnltr.Reset( \
	    &sm[0], \
	    live_rec_buf_len);
    	grnltr.Dispatch(0);
      }
      break;
    default:
      break;
  }
}

void Controls()
{
  float k1, k2;

  k1 = knob1.Process();
  k2 = knob2.Process();
  
  grnltr_params.GrainPitch =   pitch_p.Process(k1, cur_page);
  if (mmh.GotClock()) {
    rate_p.Set((mmh.GetBPM() / sample_bpm));
  }
  grnltr_params.ScanRate =     rate_p.Process(k2, cur_page);
  grnltr_params.GrainDur =     grain_duration_p.Process(k1, cur_page);
  grnltr_params.GrainDens =    (int32_t)grain_density_p.Process(k2, cur_page);
  grnltr_params.ScatterDist =  scatter_dist_p.Process(k1, cur_page);
  grnltr_params.PitchDist =    pitch_dist_p.Process(k1, cur_page);
  grnltr_params.SampleStart =  sample_start_p.Process(k1, cur_page);
  grnltr_params.SampleEnd =    sample_end_p.Process(k2, cur_page);
  grnltr_params.Crush =        crush_p.Process(k1, cur_page);
  grnltr_params.DownSample =   downsample_p.Process(k2, cur_page);
}

float pod_init() {
  hw.Init();
  return hw.AudioSampleRate();
}

void pod_start(AudioHandle::AudioCallback cb) {
  knob1.Init(hw.knob1, 0.0f, 1.0f, knob1.LINEAR);
  knob2.Init(hw.knob2, 0.0f, 1.0f, knob2.LINEAR);
  hw.StartAdc();
  hw.StartAudio(cb);
  hw.midi.StartReceive();
}
