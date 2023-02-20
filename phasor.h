#pragma once

#define MIN_SAMP 48

class Phasor
{
  public:

    Phasor() {}
    ~Phasor() {} 

    void Init(float sr, size_t len) 
    {
      sr_ = sr;
      len_ = end_pos_ = len;
      start_pos_ = loop_pos_ = 0;
      cur_pos_ = 0;
      phase_incr_ = 1;
      loop_ = ping_pong_ = false;
      play_ = true;
    }

    // pitch usually in the range of 0.5 to 2.0 (-8va to +8va)
    // 1 is no pitch adjustment
    void SetPitch(float pitch)
    {
      phase_incr_ = pitch;
    }

    // Set a specific frequency in hertz
    void SetFreq(float freq)
    {
      phase_incr_ = sr_ / (freq * len_);
    }

    // Set a duration in seconds
    void SetDur(float dur)
    {
      phase_incr_ = len_ / (dur * sr_);
    }

    // All pos are in the range 0 to 1
    // They will be multiplied by len_ to get actual size_t sample position
    
    // Don't let start_pos_ go past end_pos_
    void SetStartPos(float pos)
    {
      size_t tmp_pos;
      tmp_pos = (pos < 0.0f) ? 0 : ((pos > 1.0f) ? len_ : (size_t)(pos * len_));
      start_pos_ = (tmp_pos >= (end_pos_ - MIN_SAMP)) ? (end_pos_ - MIN_SAMP) : tmp_pos;
    }

    // Set cur_pos_ exactly
    // don't need to check for < 0 as size_t is unsigned?
    void SetCurPos(size_t pos)
    {
      cur_pos_ = (pos > len_) ? len_ : pos;
    }

    // Don't let end_pos_ get in front of start_pos_
    void SetEndPos(float pos)
    {
      size_t tmp_pos;
      
      tmp_pos = (pos < 0.0f) ? 0 : ((pos > 1.0f) ? len_ : (size_t)(pos * len_));
      end_pos_ = (tmp_pos <= (start_pos_ + MIN_SAMP)) ? (start_pos_ + MIN_SAMP) : tmp_pos;
    }

    // No checking is done here to ensure loop_pos_ is between start_pos_ and end_pos_
    // The logic for this should be done in Process()
    void SetLoopPos(float pos)
    {
      loop_pos_ = (pos < 0.0f) ? 0 : ((pos > 1.0f) ? len_ : (size_t)(pos * len_));
    }

    void Reset()
    {
      if (reverse_) {
	cur_pos_ = end_pos_;
      } else {
	cur_pos_ = start_pos_;
      }
      play_ = true;
    }


    void ToggleLoop()
    {
      loop_ = loop_ ? false : true;
      play_ = true;
    }

    void TogglePingPong()
    {
      ping_pong_ = ping_pong_ ? false : true;
      reverse_ = false;
      play_ = true;
    }

    void SetReverse(bool r)
    {
      if (r != reverse_) {
	ToggleReverse();
      }
    }

    void ToggleReverse()
    {
      reverse_ = reverse_ ? false : true;
      if (reverse_) {
	cur_pos_ = end_pos_;
      } else {
	cur_pos_ = start_pos_;
      }
      play_ = true;
    }

    inline float GetPos()
    {
      return cur_pos_;
    }
    
    float Process(bool *eot)
    {
      *eot = false;
      if (play_) {
	if (reverse_) {
      	  cur_pos_ -= phase_incr_;
      	  if (cur_pos_ < start_pos_) {
	    *eot = true;
	    if (ping_pong_) {
	      cur_pos_ += phase_incr_;
	      reverse_ = false;
	    } else if (loop_) {
      	      cur_pos_ += (end_pos_ - start_pos_);
      	    } else {
	      play_ = false;
	    }
      	  }
      	} else {
      	  cur_pos_ += phase_incr_;
	  // is it >= or just > ?
      	  if (cur_pos_ >= end_pos_) {
	    *eot = true;
	    if (ping_pong_) {
	      cur_pos_ -= phase_incr_;
	      reverse_ = true;
	    } else if (loop_) {
      	      cur_pos_ -= (end_pos_ - start_pos_);
      	    } else {
	      play_ = false;
	    }
      	  }
      	} 
      } 
      return cur_pos_;
    }


  protected:
    bool loop_, ping_pong_, reverse_, play_;
    size_t start_pos_, end_pos_, loop_pos_, len_;
    float sr_, cur_pos_, phase_incr_;
};

