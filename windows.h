#pragma once
void gaussian_window(float *mem, size_t len, float sigma);
void rectangular_window(float *mem, size_t len);
void triangular_window(float *mem, size_t len);
void hann_window(float *mem, size_t len);
void sine_window(float *mem, size_t len);

#define EQUIRIPPLE_HAMMING_COEF 0.53836 
void hamming_window(float *mem, size_t len, float a0);

//These windows need more work, they are clicky
#define  BLACKMAN_COEFS(X)      ((1-X)/2),  -0.5,        (X/2),	     0,		  false
#define  NUTALL_COEFS           0.355768,   0.487396,    0.144232,   0.012604,    true
#define  BLACKMAN_NUTALL_COEFS  0.3635819,  -0.4891775,  0.1365995,  -0.0106411,  false
#define  BLACKMAN_HARRIS_COEFS  0.35875,    -0.48829,    0.14128,    -0.01168,    false
void blackman_var_window(float *mem, size_t len, float a0, float a1, float a2, float a3, bool symmetric);


#define TAU 0.25
void expodec_window(float *expo, float *rexpo, size_t len, float tau);
