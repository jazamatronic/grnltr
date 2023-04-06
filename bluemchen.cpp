#include "kxmx_bluemchen.h"
#include "bluemchen.h"
#include "EventQueue.h"

kxmx::Bluemchen hw;
static Parameter knob1, knob2;
uint32_t last_ui_update = 0;
float long_press = false;
bool  param_select = false;
int8_t cur_param = 0;


page_t pages[NUM_PAGES] = { 
  {"RED",     "grain env",  "rst pitch",  eq.INCR_GRAIN_ENV,  eq.RST_PITCH_SCAN}, 
  {"ORANGE",  "grain rev",  "scan rev",	  eq.TOG_GRAIN_REV,   eq.TOG_SCAN_REV}, 
  {"YELLOW",  "scatter",    "freeze",	  eq.TOG_SCAT,	      eq.TOG_FREEZE}, 
  {"GREEN",   "rnd pitch",  "rnd dens",	  eq.TOG_RND_PITCH,   eq.TOG_RND_DENS}, 
  {"BLUE",    "live rec",   "play rec",	  eq.LIVE_REC,	      eq.LIVE_PLAY}, 
  {"PURPLE",  "loop",	    "",		  eq.TOG_LOOP,	      eq.NONE} 
};

void UpdateEncoder(int8_t cur_page)
{
  if (hw.encoder.Pressed()) {
    if (!long_press) {
      if (hw.encoder.TimeHeldMs() > LONG_PRESS) {
        long_press = true;
        param_select = !param_select;
      }
    }
  }

  if(hw.encoder.FallingEdge()) {
    if (long_press) {
      long_press = false;
    } else {
      if (param_select) {
	eq.push_event(pages[cur_page].events[cur_param], 0);
      } else {
        eq.push_event(eq.INCR_WAV, 0);
      }
    }
  }

  int32_t incr = hw.encoder.Increment();

  if (incr == 0) { return; }
  if (param_select) {
    cur_param = cur_param + incr;
    if (cur_param > 1) {cur_param = 0;}
    if (cur_param < 0) {cur_param = 1;}
  } else {
    (incr > 0) ? \
      eq.push_event(eq.PAGE_UP, 0) : \
      eq.push_event(eq.PAGE_DN, 0);
  }
}

void UpdateUI(int8_t cur_page)
{
  // limit update rate
  uint32_t now = hw.seed.system.GetNow();
  if (last_ui_update - now >= 30) {
    last_ui_update = now;

    hw.display.Fill(false);

    hw.display.SetCursor(0, 0);
    hw.display.WriteString(pages[cur_page].page, Font_6x8, true);
    hw.display.SetCursor(0, 10);
    hw.display.WriteString(pages[cur_page].param[0], Font_6x8, !(param_select && (cur_param == 0)));
    hw.display.SetCursor(0, 20);
    hw.display.WriteString(pages[cur_page].param[1], Font_6x8, !(param_select && (cur_param == 1)));

    hw.display.Update();
  }
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
