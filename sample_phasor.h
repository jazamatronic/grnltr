#pragma once

#include "phasor.h"

template <typename T>
class Sample : public Phasor
{
  public:
    Sample() {}
    ~Sample() {} 

    void Init(T *start, float sr, size_t len) 
    {
      mem_start_ = start;
      Phasor::Init(sr, len);
    }

    // danger = it should be the same length as the original sample otherwise the phasor won't work
    void SetSample(T *start)
    {
      mem_start_ = start;
    }

    float Process(bool *eot)
    {
      size_t idx0, idx1;
      T s0, s1;
      float sf, sample;
    
      if (play_) {
	idx0 = (size_t)cur_pos_;
      	if (reverse_) {
      	  idx1 = (idx0 == start_pos_) ? end_pos_ : idx0 - 1;
      	} else {
      	  idx1 = (idx0 == end_pos_) ? start_pos_ : idx0 + 1;
      	}
      	sf = cur_pos_ - idx0;
    
      	s0 = mem_start_[idx0];
      	s1 = mem_start_[idx1];
    
      	switch(sizeof(T)) 
      	{
      	  case 2:
      	    sample = s162f(T(s0 + sf * (s1 - s0)));
      	    break;
      	  case 4:
      	    sample = s0 + sf * (s1 - s0);
      	    break;
      	  default:
      	    // TODO: deal with other sample formats
      	    break;
      	}
     
      } else {
	sample = 0.0f;
      }

      Phasor::Process(eot);
    
      return sample;
    }

  private:
    T *mem_start_;
};
