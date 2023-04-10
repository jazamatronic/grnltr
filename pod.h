#pragma once

#define HW_TYPE daisy::DaisyPod

#include "daisy_pod.h"
#include "params.h"
#include "MidiMsgHandler.h"
#include "EventQueue.h"
#include "grnltr.h"
#include "status.h"

#define LONG_PRESS 512

extern float sample_bpm;
extern MidiMsgHandler<HW_TYPE> mmh;
extern EventQueue<QUEUE_LENGTH> eq;
extern uint8_t	dir_count;
extern int8_t	cur_dir;

extern DaisyPod hw;

float hw_init();
void hw_start(AudioHandle::AudioCallback cb);
void UpdateEncoder(int8_t cur_page);
void UpdateUI(int8_t cur_page);
void UpdateButtons(int8_t cur_page);
void Controls(int8_t cur_page);
void Status(status_t status);
