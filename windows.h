#pragma once
void gaussian_window(float *mem, size_t len, float sigma);
void rectangular_window(float *mem, size_t len);
void triangular_window(float *mem, size_t len);
void hann_window(float *mem, size_t len);
void sine_window(float *mem, size_t len);

#define EQUIRIPPLE_HAMMING_COEF 0.53836f 
void hamming_window(float *mem, size_t len, float a0);

//These windows need more work, they are clicky
#define  BLACKMAN_COEFS(X)      ((1.0f-X)/2),  -0.5f,        (X/2),	     0,		  false
#define  NUTALL_COEFS           0.355768f,   0.487396f,    0.144232f,   0.012604f,    true
#define  BLACKMAN_NUTALL_COEFS  0.3635819f,  -0.4891775f,  0.1365995f,  -0.0106411f,  false
#define  BLACKMAN_HARRIS_COEFS  0.35875f,    -0.48829f,    0.14128f,    -0.01168f,    false
void blackman_var_window(float *mem, size_t len, float a0, float a1, float a2, float a3, bool symmetric);


#define TAU 0.25f
void expodec_window(float *expo, float *rexpo, size_t len, float tau);
