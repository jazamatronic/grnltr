#pragma once

#include "PagedParam.h"

#define NUM_PAGES 9

#define DEFAULT_GRAIN_DUR 0.05f 
#define MIN_GRAIN_DUR 0.01f 
#define MAX_GRAIN_DUR 0.2f 
#define DEFAULT_GRAIN_PITCH 1.0f 
#define MIN_GRAIN_PITCH 0.25f
#define MAX_GRAIN_PITCH 4.0f
#define DEFAULT_SCAN_RATE 1.0f 
#define MIN_SCAN_RATE 0.25f
#define MAX_SCAN_RATE 4.0f
#define DEFAULT_GRAIN_DENS 60 
#define MIN_GRAIN_DENS 200 
#define MAX_GRAIN_DENS 2 
#define DEFAULT_SCATTER_DIST  0.1f 
#define DEFAULT_PITCH_DIST    0.1f 
#define DEFAULT_GRAIN_VOL     0.7f
#define DEFAULT_PAN	      0.5f
#define DEFAULT_PAN_DIST      0.5f
#define DEFAULT_MIX	      0.0f
#define DEFAULT_DLY	      0.5f
#define DEFAULT_FBK	      0.0f
#define DEFAULT_XST	      0.0f

#define MAX_DELAY static_cast<size_t>(48000)


#define PARAM_THRESH 0.01f
extern PagedParam pitch_p, rate_p, crush_p, downsample_p, grain_duration_p, \
		  grain_density_p, scatter_dist_p, pitch_dist_p, sample_start_p, \
		  sample_end_p, pan_p, pan_dist_p, dly_mix_p, dly_time_p, \
		  dly_fbk_p, dly_xst_p;

typedef struct {
  float	  GrainPitch;
  float   ScanRate;
  float   GrainDur;
  float	  ScatterDist;
  float	  PitchDist;
  float	  SampleStart;
  float	  SampleEnd;
  float	  Crush;
  float	  DownSample;
  float	  Pan;
  float	  PanDist;
  float	  DelayMix;
  float	  DelayTime;
  float	  DelayFbk;
  float	  DelayXSt;
  int32_t GrainDens;
} grnltr_params_t;

extern grnltr_params_t grnltr_params;
