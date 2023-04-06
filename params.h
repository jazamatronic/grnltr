#pragma once

#include "PagedParam.h"

#define DEFAULT_GRAIN_DUR 0.2f 
#define MIN_GRAIN_DUR 0.01f 
#define MAX_GRAIN_DUR 0.2f 
#define DEFAULT_GRAIN_PITCH 1.0f 
#define MIN_GRAIN_PITCH 0.25f
#define MAX_GRAIN_PITCH 4.0f
#define DEFAULT_SCAN_RATE 1.0f 
#define MIN_SCAN_RATE 0.25f
#define MAX_SCAN_RATE 4.0f
#define DEFAULT_GRAIN_DENS 10 
#define MIN_GRAIN_DENS 200 
#define MAX_GRAIN_DENS 2 
#define DEFAULT_SCATTER_DIST  0.1f 
#define DEFAULT_PITCH_DIST    0.1f 
#define DEFAULT_GRAIN_VOL     0.7f

#define PARAM_THRESH 0.01f
extern PagedParam pitch_p, rate_p, crush_p, downsample_p, grain_duration_p, \
		  grain_density_p, scatter_dist_p, pitch_dist_p, sample_start_p, \
	    	  sample_end_p;

typedef struct {
  float	  GrainPitch;
  float   ScanRate;
  float   GrainDur;
  int32_t GrainDens;
  float	  ScatterDist;
  float	  PitchDist;
  float	  SampleStart;
  float	  SampleEnd;
  float	  Crush;
  float	  DownSample;
} grnltr_params_t;

extern grnltr_params_t grnltr_params;
