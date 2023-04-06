#pragma once

#define HW_TYPE daisy::DaisyPod

#include "daisy_pod.h"
#include "params.h"
#include "MidiMsgHandler.h"
#include "EventQueue.h"

extern float sample_bpm;
extern float sr;
extern MidiMsgHandler<HW_TYPE> mmh;
extern EventQueue<QUEUE_LENGTH> eq;

extern DaisyPod hw;

float hw_init();
void hw_start(AudioHandle::AudioCallback cb);
void UpdateEncoder();
void UpdateUI(int8_t cur_page);
void UpdateButtons(int8_t cur_page);
void InitControls();
void Controls(int8_t cur_page);
