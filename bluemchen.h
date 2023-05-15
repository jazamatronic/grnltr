#pragma once

#define HW_TYPE kxmx::Bluemchen

#include "kxmx_bluemchen.h"
#include "params.h"
#include "MidiMsgHandler.h"
#include "EventQueue.h"
#include "grnltr.h"
#include "status.h"

#define MAX_STRING 11 // 10 chars 6px wide + terminating \0

#define LONG_PRESS 512
#define EXTRA_LONG_PRESS (LONG_PRESS * 3)
#define DOUBLE_CLICK 500

extern float sample_bpm;
extern MidiMsgHandler<HW_TYPE> mmh;
extern EventQueue<QUEUE_LENGTH> eq;
extern char dir_names[MAX_DIRS][MAX_DIR_LENGTH];
extern uint8_t	dir_count;
extern int8_t	cur_dir;

extern kxmx::Bluemchen hw;

float hw_init();
void hw_start(AudioHandle::AudioCallback cb);
void UpdateEncoder(int8_t cur_page);
void UpdateUI(int8_t cur_page);
void Status(status_t status);

typedef struct {
  char const *page;
  char const *param[2];
  EventQueue<QUEUE_LENGTH>::event events[2];
} page_t;
