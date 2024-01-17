#pragma once
//typedef struct {
//  uint8_t num_notes;
//  float *scale;
//} scale_t;

#include <math.h>
#define SEMITONE (1.0f / 12.0f)
#define ST(x) float(x * SEMITONE)

#define NUM_SCALES 11 
float chromatic[] =	{ST(0), ST(1), ST(2), ST(3), ST(4), ST(5), ST(6), ST(7), ST(8), ST(9), ST(10), ST(11), ST(12)};
float min3_octs[] = 	{ST(0), ST(3), ST(12)};
float maj3_octs[] = 	{ST(0), ST(4), ST(12)};
float fourths_octs[] =	{ST(0), ST(5), ST(12)};
float fifths_octs[] =	{ST(0), ST(7), ST(12)};
float min[] =		{ST(0), ST(3), ST(7), ST(12)};
float min7[] =		{ST(0), ST(3), ST(7), ST(10), ST(12)};
float minM7[] =		{ST(0), ST(3), ST(7), ST(11), ST(12)};
float maj[] =		{ST(0), ST(4), ST(7), ST(12)};
float maj7[] =		{ST(0), ST(4), ST(7), ST(10), ST(12)};
float majM7[] =		{ST(0), ST(4), ST(7), ST(11), ST(12)};

class Scale
{
  public:
    //Scale(uint8_t num_notes, float *scale) {num_notes_ = num_notes; scale_ = scale;}
    Scale() {}
    ~Scale() {}

  void Init(uint8_t num_notes, float *scale)
  {
    num_notes_ = num_notes;
    scale_ = scale;
  }

  /*
   * val is a number between -1 and 1
   * Create an index to the scale by multiplying val by the number of notes and rounding
   * Take care of val when it is less than 0 - it has to be scaled by a half in this case
   */
  float Quantize(float val)
  {
    int idx = (int)roundf(fabs(val) * (num_notes_ - 1.6f));
    return (val < 0) ? -0.5f * scale_[idx] : scale_[idx];
  }

  private:
    uint8_t num_notes_;
    float *scale_;
};
#pragma once
