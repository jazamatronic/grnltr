#pragma once

#define HW_TYPE kxmx::Bluemchen

#include "kxmx_bluemchen.h"
#include "grain_envs.h"
#include "params.h"
#include "sample_mem.h"
#include "MidiMsgHandler.h"
#include "granulator.h"

#define NUM_PAGES 5

#define MAX_STRING 12

extern int8_t cur_page;
extern int8_t cur_wave;
extern float sample_bpm;
extern float sr;
extern Granulator grnltr;
extern MidiMsgHandler<HW_TYPE> mmh;
extern kxmx::Bluemchen hw;

float hw_init();
void hw_start(AudioHandle::AudioCallback cb);
void UpdateEncoder();
void UpdateScreen();
void InitControls();
void Controls();
