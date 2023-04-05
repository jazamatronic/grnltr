#pragma once

#include "PagedParam.h"

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
