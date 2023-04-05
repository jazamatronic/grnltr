#include "kxmx_bluemchen.h"
#include "bluemchen.h"

kxmx::Bluemchen hw;
static Parameter knob1, knob2;

char strings[][MAX_STRING] = {"RED", "YELLOW", "ORANGE", "GREEN", "BLUE"};

void UpdateEncoder()
{
  cur_page = (int8_t)(cur_page + hw.encoder.Increment());
  if (cur_page >= NUM_PAGES) { cur_page = 0; }
  if (cur_page < 0) { cur_page += NUM_PAGES; }

  if(hw.encoder.RisingEdge()) {
    cur_wave++;
    if (cur_wave >= wav_file_count) cur_wave = 0;
    grnltr.Stop();
    InitControls();
    grnltr.Reset( \
        &sm[wav_start_pos[cur_wave]], \
        wav_file_names[cur_wave].raw_data.SubCHunk2Size / sizeof(int16_t));
    grnltr.Dispatch(0);
  }
}

void UpdateScreen()
{
  hw.display.Fill(false);

  hw.display.SetCursor(0, 0);
  hw.display.WriteString(&strings[cur_page][0], Font_6x8, true);

  hw.display.Update();
}

/*
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
*/

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
