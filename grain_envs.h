#pragma once

#define GRAIN_ENV_SIZE 1024
#define NUM_GRAIN_ENVS 6

extern float rect_env[GRAIN_ENV_SIZE];
extern float gauss_env[GRAIN_ENV_SIZE];
extern float hamming_env[GRAIN_ENV_SIZE];
extern float hann_env[GRAIN_ENV_SIZE];
extern float expo_env[GRAIN_ENV_SIZE];
extern float rexpo_env[GRAIN_ENV_SIZE];
extern float *grain_envs[];
extern size_t cur_grain_env; 

