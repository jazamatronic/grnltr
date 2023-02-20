#include "arm_math.h"
#include "windows.h"

void gaussian_window(float *mem, size_t len, float sigma)
{
  for (size_t i = 0; i < len; i++) {
    mem[i] = exp( -0.5 * pow(((i - len/2) / (sigma * len/2)), 2));
  }
}

void rectangular_window(float *mem, size_t len)
{
  for (size_t i = 0; i < len; i++) {
    mem[i] = 1.0f;
  }
}

// something is clicky
void triangular_window(float *mem, size_t len)
{
  size_t half_len = len / 2;
  float step = 1.0f / (float)half_len;
  float val = 0.0f;
  for (size_t i = 0; i < half_len; i++) {
    mem[i] = val;
    val += step;
  }
  for (size_t i = 0; i < half_len; i++) {
    mem[i + half_len] = mem[half_len - 1 - i];
  }
 // for (size_t i = half_len; i < len; i++) {
 //   mem[i] = val;
 //   val -= step;
 // }
}

void hann_window(float *mem, size_t len)
{
  /*
   * for (size_t i = 0; i < len; i++) {
   *   mem[i] = 0.5 * (1.0 - cos(2 * M_PI * i / len));
   * }
   */
  hamming_window(mem, len, 0.5);
}

void hamming_window(float *mem, size_t len, float a0)
{
  float a1 = 1 - a0;
  for (size_t i = 0; i < len; i++) {
    mem[i] = a0 - a1 * cos(2.0 * M_PI * i / len);
  }
}

//		    a0		a1	    a2		a3
// nutall	    0.355768,	-0.487396,   0.144232,	-0.012604
// blackman-nutall  0.3635819,	0.4891775,  0.1365995,  0.0106411
// blackman-harris  0.35875,	0.48829,    0.14128,	0.01168
void blackman_var_window(float *mem, size_t len, float a0, float a1, float a2, float a3, bool symmetric)
{
  int16_t idx;
  for (size_t i = 0; i < len; i++) {
    idx = symmetric ? i - (len / 2) : i;
    mem[i] = a0 \
	     - a1 * cos(2.0 * M_PI * (float)idx / (len - 1)) \
	     + a2 * cos(4.0 * M_PI * (float)idx / (len - 1)) \
	     - a3 * cos(6.0 * M_PI * (float)idx / (len - 1));
  }
}

void expodec_window(float *expo, float *rexpo, size_t len, float tau)
{
  float n;
  float offset = exp(-1.0 / tau);
  for (size_t i = 0; i < len; i++) {
    n = expf(-((float)i / len) / tau) - offset;
    // yes, I could just play it in reverse, and may do so if I run out of memory
    rexpo[i] = n;
    expo[len - 1 - i] = n;
  }
}
