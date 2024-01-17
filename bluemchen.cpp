#include "kxmx_bluemchen.h"
#include "bluemchen.h"
#include "EventQueue.h"
#include "grnltr.h"

kxmx::Bluemchen hw;
extern Parameter knob1, knob2;
uint32_t last_ui_update = 0;
bool long_press = false;
bool extra_long_press = false;
bool param_select = false;
bool setup_page = false;
bool browse = false;
int8_t cur_param = 0;
int8_t next_dir;


page_t pages[NUM_PAGES] = { 
  {"RED",     "grain env",  "rst pitch",  eq.INCR_GRAIN_ENV,  eq.RST_PITCH_SCAN},  /* k1 = Grain Pitch	    k2 = Scan Rate*	*/
  {"ORANGE",  "grain rev",  "scan rev",	  eq.TOG_GRAIN_REV,   eq.TOG_SCAN_REV},	   /* k1 = Grain Duration   k2 = Grain Density	*/
  {"YELLOW",  "scatter",    "freeze",	  eq.TOG_SCAT,	      eq.TOG_FREEZE},	   /* k1 = Scatter Distance			*/
  {"GREEN",   "rnd pitch",  "rnd dens",	  eq.TOG_RND_PITCH,   eq.TOG_RND_DENS},    /* k1 = Pitch Distance   k2 = Quantize scale */
  {"BLUE",    "live rec",   "play rec",	  eq.LIVE_REC,	      eq.LIVE_PLAY},       /* k1 = Sample Start*    k2 = Sample End*	*/
  {"PURPLE",  "",	    "",		  eq.NONE,	      eq.NONE},            /* k1 = Bit Crush	    k2 = Downsample	*/
  {"VIOLET",  "rnd pan",    "",		  eq.TOG_RND_PAN,     eq.NONE},            /* k1 = pan		    k2 = pan dist	*/
  {"ROSE",    "gate",	    "retrig",	  eq.TOG_GATE,	      eq.TOG_RETRIG},      /* k1 = delay mix	    k2 = delay time	*/
  {"LGREEN",  "note",	    "loop",	  eq.TOG_NOTE,	      eq.TOG_LOOP}         /* k1 = delay fbk	    k2 = delay xst  	*/
};

page_t setup = {"SETUP", "midi", "browse", eq.INCR_MIDI, eq.NONE};

void UpdateEncoder(int8_t cur_page)
{
  if (hw.encoder.Pressed()) {
    if (!long_press) {
      if (hw.encoder.TimeHeldMs() > LONG_PRESS) {
        long_press = true;
        param_select = !param_select;
	cur_param = 0;
	if (setup_page) {
	  param_select = false;
	  extra_long_press = false;
	  setup_page = false;
	}
      }
    } else if (!extra_long_press) {
      if (hw.encoder.TimeHeldMs() > EXTRA_LONG_PRESS) {
        extra_long_press = true;
        param_select = true;
	setup_page = true;
      }
    }
  }

  if(hw.encoder.FallingEdge()) {
    if (long_press || extra_long_press) {
      long_press = false;
      extra_long_press = false;
    } else {
      if (browse) {
	browse = false;
	if (next_dir != cur_dir) {
	  eq.push_event(eq.NEXT_DIR, next_dir);
	}
      } else if (setup_page) {
	if (cur_param) {
	  browse = true;
	  next_dir = cur_dir;
	} else {
	  eq.push_event(setup.events[cur_param], 0);
	}
      } else if (param_select) {
	eq.push_event(pages[cur_page].events[cur_param], 0);
      } else {
        eq.push_event(eq.INCR_WAV, 0);
      }
    }
  }

  int32_t incr = hw.encoder.Increment();

  if (incr == 0) { return; }
  if (browse) {
    next_dir += incr;
    if (next_dir >= dir_count) next_dir = 0;
    if (next_dir < 0) next_dir += dir_count;
  } else if (param_select || setup_page) {
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
  page_t *page;
  int8_t dir_idx = next_dir;
  char disp[MAX_STRING + 1]; //+1 otherwise g++ complains?

  // limit update rate
  uint32_t now = hw.seed.system.GetNow();
  if (last_ui_update - now >= 30) {
    last_ui_update = now;

    if (setup_page) {
      page = &setup;
    } else {
      page = &pages[cur_page];
    }

    hw.display.Fill(false);

    if (browse) {
      hw.display.SetCursor(0, 0);
      strncpy(disp, &dir_names[dir_idx][0], MAX_STRING);
      hw.display.WriteString(disp, Font_6x8, false);
      if (++dir_idx < dir_count) {
	hw.display.SetCursor(0, 10);
	strncpy(disp, &dir_names[dir_idx][0], MAX_STRING);
      	hw.display.WriteString(disp, Font_6x8, true);
	if (++dir_idx < dir_count) {
	  hw.display.SetCursor(0, 20);
	  strncpy(disp, &dir_names[dir_idx][0], MAX_STRING);
      	  hw.display.WriteString(disp, Font_6x8, true);
	}
      }
    } else {
      hw.display.SetCursor(0, 0);
      hw.display.WriteString(page->page, Font_6x8, true);
      hw.display.SetCursor(0, 10);
      hw.display.WriteString(page->param[0], Font_6x8, !(param_select && (cur_param == 0)));
      hw.display.SetCursor(0, 20);
      hw.display.WriteString(page->param[1], Font_6x8, !(param_select && (cur_param == 1)));
    }

    hw.display.Update();
  }
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

void Status(status_t status)
{
  hw.display.Fill(false);
  hw.display.SetCursor(0, 0);
  switch(status) 
  {
    case STARTUP:
      hw.display.WriteString("Manifest", Font_6x8, true);
      hw.display.SetCursor(0, 10);
      hw.display.WriteString("awareness", Font_6x8, true);
      hw.display.SetCursor(0, 20);
      hw.display.WriteString(VER, Font_6x8, true);
      break;
    case MOUNT_ERROR:
      hw.display.WriteString("SDCARD?", Font_6x8, true);
      break;
    case NO_GRNLTR_DIR:
      hw.display.WriteString("GRNLTR?", Font_6x8, true);
      break;
    case DIR_ERROR:
      hw.display.WriteString("DIR?", Font_6x8, true);
      break;
    case NO_WAVS:
      hw.display.WriteString("WAVS?", Font_6x8, true);
      break;
    case READING_WAV:
      hw.display.WriteString("LOADING..", Font_6x8, true);
      break;
    case MISSING_WAV:
      hw.display.WriteString("SIZE?", Font_6x8, true);
      break;
    case FSI_INIT:
      hw.display.WriteString("~.~", Font_6x8, true);
      break;
    case SD_MOUNT:
      hw.display.WriteString("/o/", Font_6x8, true);
      break;
    case LIST_DIRS:
      hw.display.WriteString("|O|", Font_6x8, true);
      break;
    case OK:
      break;
    case GRNLTR_INIT:
      hw.display.WriteString("Done.", Font_6x8, true);
      break;
    default:
      break;
  }
  hw.display.Update();
}
