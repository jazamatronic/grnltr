#include "pod.h"
#include "led_colours.h"
#include "EventQueue.h"

DaisyPod hw;
static Parameter knob1, knob2;
uint32_t last_ui_update = 0;
bool long_press = false;
bool setup_page = false;
int8_t next_dir;

void UpdateEncoder(int8_t cur_page)
{
  if (hw.encoder.Pressed()) {
    if (!long_press) {
      if (hw.encoder.TimeHeldMs() > LONG_PRESS) {
        long_press = true;
	setup_page = !setup_page;
      }
    }
  }

  if(hw.encoder.FallingEdge()) {
    if (long_press) {
      long_press = false;
    } else {
      if (setup_page) {
	// button presses handled below
      } else {
        eq.push_event(eq.INCR_WAV, 0);
      }
    }
  }

  int32_t incr = hw.encoder.Increment();

  if (incr == 0) { return; }
  if (setup_page) {
    // nothing here button presses handled below
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
  if (last_ui_update - now >= 1) {
    last_ui_update = now;

    if (setup_page) {
      hw.led1.Set(CYAN);
    } else {
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
           * b2 = Toggle Retrig (disabled in live mode)
           */
          break;
        case 6:
          hw.led1.Set(VIOLET);
          /*
           * k1 = pan
           * k2 = pan dist
           * b1 = Toggle Gate (disabled in live mode)
           * b2 = Toggle random pan
           */
          break;
        case 7:
          hw.led1.Set(ROSE);
          /*
           * k1 = delay mix
           * k2 = delay time
           */
          break;
        case 8:
          hw.led1.Set(LGREEN);
          /*
           * k1 = delay fbk
           * k2 = delay xst 
           */
          break;
        default:
          hw.led1.Set(WHITE);
          break;
      }
    }
    hw.UpdateLeds();
  }
}

void UpdateButtons(int8_t cur_page)
{

  if (setup_page) {
    if(hw.button1.RisingEdge()) {
      eq.push_event(eq.INCR_MIDI, 0);
    }
    if(hw.button2.RisingEdge()) {
      next_dir = cur_dir;
      next_dir++;
      if (next_dir >= dir_count) next_dir = 0;
      eq.push_event(eq.NEXT_DIR, next_dir);
    }
    return;
  }

  switch(cur_page)
  {
    case 0:
      if(hw.button1.RisingEdge()) {
	eq.push_event(eq.INCR_GRAIN_ENV, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(eq.RST_PITCH_SCAN, 0);
      }
      break;
    case 1:
      if(hw.button1.RisingEdge()) {
	eq.push_event(eq.TOG_GRAIN_REV, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(eq.TOG_SCAN_REV, 0);
      }
      break;
    case 2:
      if(hw.button1.RisingEdge()) {
	eq.push_event(eq.TOG_SCAT, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(eq.TOG_FREEZE, 0);
      }
      break;
    case 3:
      if(hw.button1.RisingEdge()) {
	eq.push_event(eq.TOG_RND_PITCH, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(eq.TOG_RND_DENS, 0);
      }
      break;
    case 4:
      if(hw.button1.RisingEdge()) {
	eq.push_event(eq.LIVE_REC, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(eq.LIVE_PLAY, 0);
      }
      break;
    case 5:
      if(hw.button1.RisingEdge()) {
	eq.push_event(eq.TOG_LOOP, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(eq.TOG_RETRIG, 0);
      }
      break;
    case 6:
      if(hw.button1.RisingEdge()) {
	eq.push_event(eq.TOG_GATE, 0);
      }
      if(hw.button2.RisingEdge()) {
	eq.push_event(eq.TOG_RND_PAN, 0);
      }
      break;
    default:
      break;
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
  grnltr_params.Pan =	       pan_p.Process(k1, cur_page);
  grnltr_params.PanDist =      pan_dist_p.Process(k2, cur_page);
  grnltr_params.DelayMix =     dly_mix_p.Process(k1, cur_page);
  grnltr_params.DelayTime =    dly_time_p.Process(k2, cur_page);
  grnltr_params.DelayFbk =     dly_fbk_p.Process(k1, cur_page);
  grnltr_params.DelayXSt =     dly_xst_p.Process(k2, cur_page);
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

void Status(status_t status)
{
  switch(status) 
  {
    case STARTUP:
      hw.led1.Set(RED);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("GRNLTR %s", VER);
      hw.seed.PrintLine("SD CFG");
    #endif
      break;
    case FSI_INIT:
      hw.led1.Set(ORANGE);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("  OK");
      hw.seed.PrintLine("FSI Init");
    #endif
      break;
    case SD_MOUNT:
      hw.led1.Set(YELLOW);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("  OK");
      hw.seed.PrintLine("SD Mount");
    #endif
      break;
    case MOUNT_ERROR:
      hw.led2.Set(CYAN);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("  FAILED!");
    #endif
      break;
    case LIST_DIRS:
      hw.led1.Set(GREEN);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("  OK");
      hw.seed.PrintLine("Searching %s", GRNLTR_PATH);
    #endif
      break;
    case NO_GRNLTR_DIR:
      hw.led2.Set(LGREEN);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("%s not found or empty!", GRNLTR_PATH);
    #endif
      break;
    case DIR_ERROR:
      hw.led2.Set(WHITE);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("  DIR read error");
    #endif
      break;
    case NO_WAVS:
      hw.led2.Set(ROSE);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("  No WAVs found");
    #endif
      break;
    case READING_WAV:
      hw.led1.Set(BLUE);
      break;
    case MISSING_WAV:
      hw.led2.Set(LBLUE);
      break;
    case OK:
      hw.led1.Set(OFF);
      hw.led2.Set(OFF);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("  OK");
    #endif
      break;
    case GRNLTR_INIT:
      hw.led1.Set(PURPLE);
    #ifdef DEBUG_POD
      hw.seed.PrintLine("Initializing GRNLTR");
    #endif
      break;
    default:
      break;
  }
  hw.UpdateLeds();
}
