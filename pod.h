#pragma once

#include "grain_envs.h"
#include "params.h"
#include "sample_mem.h"
#include "MidiMsgHandler.h"
#include "granulator.h"

#define NUM_PAGES 6

extern int8_t cur_page;
extern int8_t cur_wave;
extern float sample_bpm;
extern Granulator grnltr;
extern MidiMsgHandler mmh;
extern DaisyPod hw;

float pod_init();
void pod_start(AudioHandle::AudioCallback cb);
void UpdateEncoder();
void UpdateButtons();
void Controls();
extern void InitControls();
