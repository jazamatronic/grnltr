#pragma once

#define HW_TYPE kxmx::Bluemchen

#include "kxmx_bluemchen.h"
#include "params.h"
#include "MidiMsgHandler.h"
#include "EventQueue.h"

#define MAX_STRING 12

#define LONG_PRESS 512

extern float sample_bpm;
extern MidiMsgHandler<HW_TYPE> mmh;
extern EventQueue<QUEUE_LENGTH> eq;

extern kxmx::Bluemchen hw;

float hw_init();
void hw_start(AudioHandle::AudioCallback cb);
void UpdateEncoder(int8_t cur_page);
void UpdateUI(int8_t cur_page);
void Controls(int8_t cur_page);

typedef struct {
  char const *page;
  char const *param[2];
  EventQueue<QUEUE_LENGTH>::event events[2];
} page_t;
