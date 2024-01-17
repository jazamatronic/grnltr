#pragma once

#include "util/wav_format.h"

#define GRAIN_ENV_SIZE 1024
#define NUM_GRAIN_ENVS 6
#define DEFAULT_GRAIN_ENV 2 // hamming

#define GRNLTR_PATH	"/grnltr"
#define MAX_DIRS	64
#define MAX_DIR_LENGTH  64
#define MAX_WAVES	16
#define LINE_BUF_SIZE	128

#define CP_BUF_SIZE 8192

#define DEFAULT_BPM 120.0f

#define MIDI_CHANNEL	    0 // todo - make this settable somehow. Daisy starts counting MIDI channels from 0
#define CC_SCAN	       	    1 // MOD wheel controls Scan Rate
#define CC_GRAINPITCH	    3 // CC3 and PitchBend control GrainPitch
#define CC_GRAINDUR	    9 
#define	CC_GRAINDENS	    14  
#define CC_TOG_SCATTER	    15
#define CC_SCATTERDIST	    20
#define CC_TOG_PITCH	    21
#define CC_PITCHDIST	    22
#define CC_SAMPLESTART_MSB  12
#define CC_SAMPLESTART_LSB  44
#define CC_SAMPLEEND_MSB    13
#define CC_SAMPLEEND_LSB    45
#define	CC_CRUSH	    23
#define	CC_DOWNSAMPLE	    24
#define CC_TOG_GREV	    25
#define CC_TOG_SREV	    26
#define CC_TOG_FREEZE	    27
#define CC_TOG_LOOP	    28
#define CC_TOG_DENS	    29
#define	CC_BPM		    30
#define	CC_LIVE_REC	    31
#define	CC_LIVE_SAMP	    32
#define	CC_PAN		    33
#define	CC_PAN_DIST	    34
#define	CC_TOG_RND_PAN	    35
#define	CC_TOG_RETRIG	    36
#define	CC_TOG_GATE	    37
#define	CC_DLY_MIX	    38
#define	CC_DLY_TIME	    39
#define	CC_DLY_FBK	    40
#define	CC_DLY_XST	    41
#define	CC_NOTE		    42
#define CC_GRAINENV	    43
#define CC_RST_PITCH_SCAN   46
#define CC_SCALE	    47
//C3
#define BASE_NOTE	    60

// 33mS - something like 30Hz
#define MAIN_LOOP_DLY	   33 

typedef struct {
  WavFileInfo wav_file_hdr;
  size_t      wav_start_pos;
  float	      bpm;
  bool	      loop;
  bool	      rev;
} wav_info_t;

