#pragma once

#include "util/wav_format.h"
#include "hid/wavplayer.h"

#define MAX_WAVES 16

/**< Maximum LFN (set to same in FatFs (ffconf.h) */
#define WAV_FILENAME_MAX 256 

extern int16_t sm[(64 * 1024 * 1024) / sizeof(int16_t)];
extern size_t sm_size;

extern size_t cur_sm_bytes;
extern size_t live_rec_buf_len;

using namespace daisy;

extern WavFileInfo wav_file_names[MAX_WAVES];
extern uint8_t	   wav_file_count;
extern size_t	   wav_start_pos[MAX_WAVES];
