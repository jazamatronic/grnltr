#include "kxmx_bluemchen.h"
#include "bluemchen.h"
#include "EventQueue.h"

kxmx::Bluemchen hw;
static Parameter knob1, knob2;
uint8_t last_ui_update = 0;

char strings[][MAX_STRING] = {"RED", "ORANGE", "YELLOW", "GREEN", "BLUE", "PURPLE"};

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
  if (last_ui_update - now >= 30) {
    last_ui_update = now;

    hw.display.Fill(false);

    hw.display.SetCursor(0, 0);
    hw.display.WriteString(&strings[cur_page][0], Font_6x8, true);

    hw.display.Update();
  }
}

void InitControls()
{
  pitch_p.Init(           0,  DEFAULT_GRAIN_PITCH,	MIN_GRAIN_PITCH,  MAX_GRAIN_PITCH,  PARAM_THRESH);
  rate_p.Init(            0,  DEFAULT_SCAN_RATE,        MIN_SCAN_RATE,	  MAX_SCAN_RATE,    PARAM_THRESH);
  grain_duration_p.Init(  1,  DEFAULT_GRAIN_DUR,        MIN_GRAIN_DUR,    MAX_GRAIN_DUR,    PARAM_THRESH);
  grain_density_p.Init(   1,  sr/DEFAULT_GRAIN_DENS,  sr/MIN_GRAIN_DENS, sr/MAX_GRAIN_DENS, PARAM_THRESH);
  scatter_dist_p.Init(    2,  DEFAULT_SCATTER_DIST,	0.0f,   1.0f, PARAM_THRESH);
  pitch_dist_p.Init(      2,  DEFAULT_PITCH_DIST,       0.0f,   1.0f, PARAM_THRESH);
  sample_start_p.Init(    3,  0.0f,			0.0f,   1.0f, PARAM_THRESH);
  sample_end_p.Init(	  3,  1.0f,			0.0f,   1.0f, PARAM_THRESH);
  crush_p.Init(           4,  0.0f,                     0.0f,   1.0f, PARAM_THRESH);
  downsample_p.Init(      4,  0.0f,                     0.0f,   1.0f, PARAM_THRESH);
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
  knob1.Init(hw.controls[hw.CTRL_1], 0.0f, 1.0f, knob1.LINEAR);
  knob2.Init(hw.controls[hw.CTRL_2], 0.0f, 1.0f, knob2.LINEAR);
  hw.StartAdc();
  hw.StartAudio(cb);
  hw.midi.StartReceive();
}
