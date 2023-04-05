#pragma once

#define HW_TYPE daisy::DaisyPod

#include "daisy_pod.h"
#include "grain_envs.h"
#include "params.h"
#include "sample_mem.h"
#include "MidiMsgHandler.h"
#include "granulator.h"

#define NUM_PAGES 6

extern int8_t cur_page;
extern int8_t cur_wave;
extern float sample_bpm;
extern float sr;
extern Granulator grnltr;
extern MidiMsgHandler<HW_TYPE> mmh;
extern DaisyPod hw;

float hw_init();
void hw_start(AudioHandle::AudioCallback cb);
void UpdateEncoder();
void UpdateButtons();
void InitControls();
void Controls();
