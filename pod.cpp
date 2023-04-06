#include "pod.h"
#include "led_colours.h"
#include "EventQueue.h"

DaisyPod hw;
static Parameter knob1, knob2;
uint8_t last_ui_update = 0;

void UpdateEncoder()
{
  if(hw.encoder.RisingEdge()) {
    eq.push_event(EventQueue<QUEUE_LENGTH>::INCR_WAV, 0);
  }

  int32_t incr = hw.encoder.Increment();

  if (incr == 0) { return; }
  (incr > 0) ? \
    eq.push_event(EventQueue<QUEUE_LENGTH>::PAGE_UP, 0) : \
    eq.push_event(EventQueue<QUEUE_LENGTH>::PAGE_DN, 0);
}

void UpdateUI(int8_t cur_page)
{
  // limit update rate
  uint32_t now = hw.seed.system.GetNow();
  if (last_ui_update - now >= 1) {
    last_ui_update = now;

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
         * b1 = Live Mode
         * b2 = Sample Mode
         */
        break;
      case 5:
        hw.led1.Set(PURPLE);
        /*
         * k1 = Bit Crush
         * k2 = Downsample
         * b1 = Toggle Wave Loop (disabled in live mode)
         */
        break;
      default:
        hw.led1.Set(WHITE);
        break;
    }
    hw.UpdateLeds();
  }
}

void UpdateButtons(int8_t cur_page)
{
  switch(cur_page)
  {
    case 0:
      if(hw.button1.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::INCR_GRAIN_ENV, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::RST_PITCH_SCAN, 0);
      }
      break;
    case 1:
      if(hw.button1.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::TOG_GRAIN_REV, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::TOG_SCAN_REV, 0);
      }
      break;
    case 2:
      if(hw.button1.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::TOG_SCAT, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::TOG_FREEZE, 0);
      }
      break;
    case 3:
      if(hw.button1.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::TOG_RND_PITCH, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::TOG_RND_DENS, 0);
      }
      break;
    case 4:
      if(hw.button1.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::LIVE_REC, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::LIVE_PLAY, 0);
      }
      break;
    case 5:
      if(hw.button1.RisingEdge()) {
	eq.push_event(EventQueue<QUEUE_LENGTH>::TOG_LOOP, 0);
      }
      if(hw.button2.RisingEdge()) {
      }
      break;
    default:
      break;
  }
}

void InitControls()
{
  pitch_p.Init(           0,  DEFAULT_GRAIN_PITCH,	MIN_GRAIN_PITCH,  MAX_GRAIN_PITCH,  PARAM_THRESH);
  rate_p.Init(            0,  DEFAULT_SCAN_RATE,        MIN_SCAN_RATE,	  MAX_SCAN_RATE,    PARAM_THRESH);
  grain_duration_p.Init(  1,  DEFAULT_GRAIN_DUR,        MIN_GRAIN_DUR,    MAX_GRAIN_DUR,    PARAM_THRESH);
  grain_density_p.Init(   1,  sr/DEFAULT_GRAIN_DENS,  sr/MIN_GRAIN_DENS, sr/MAX_GRAIN_DENS, PARAM_THRESH);
  scatter_dist_p.Init(    2,  DEFAULT_SCATTER_DIST,	0.0f,   1.0f, PARAM_THRESH);
  pitch_dist_p.Init(      3,  DEFAULT_PITCH_DIST,       0.0f,   1.0f, PARAM_THRESH);
  sample_start_p.Init(    4,  0.0f,			0.0f,   1.0f, PARAM_THRESH);
  sample_end_p.Init(	  4,  1.0f,			0.0f,   1.0f, PARAM_THRESH);
  crush_p.Init(           5,  0.0f,                     0.0f,   1.0f, PARAM_THRESH);
  downsample_p.Init(      5,  0.0f,                     0.0f,   1.0f, PARAM_THRESH);
}

void Controls(int8_t cur_page)
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

float hw_init() {
  hw.Init();
  return hw.AudioSampleRate();
}

void hw_start(AudioHandle::AudioCallback cb) {
  knob1.Init(hw.knob1, 0.0f, 1.0f, knob1.LINEAR);
  knob2.Init(hw.knob2, 0.0f, 1.0f, knob2.LINEAR);
  hw.StartAdc();
  hw.StartAudio(cb);
  hw.midi.StartReceive();
}
