#pragma once

#define HW_TYPE kxmx::Bluemchen

#include "kxmx_bluemchen.h"
#include "params.h"
#include "MidiMsgHandler.h"
#include "EventQueue.h"

#define MAX_STRING 12

extern float sample_bpm;
extern float sr;
extern MidiMsgHandler<HW_TYPE> mmh;
extern EventQueue<QUEUE_LENGTH> eq;

extern kxmx::Bluemchen hw;

float hw_init();
void hw_start(AudioHandle::AudioCallback cb);
void UpdateEncoder();
void UpdateUI(int8_t cur_page);
void InitControls();
void Controls(int8_t cur_page);
