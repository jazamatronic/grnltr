#pragma once

#define CC_TO_VAL(x, min, max) (min + (x / 127.0f) * (max - min))
// expect a range of -1 to 1
#define PB_TO_VAL(x) (x / 8191.0f)

// All in values are expected to be in the 0 to 1 range
// cur_val is stored as a range between 0 and 1
class PagedParam
{
  public:
    PagedParam() {}
    ~PagedParam() {}

    void Init(uint8_t page, float init, float min, float max, float thresh)
    {
      page_ = page;
    
      thresh_ = thresh;
      locked_ = true;
      midi_locked_ = true;
      changed_ = false;
      min_ = min;
      max_ = max;
      range_ = max_ - min_;
    
      init_val_ = (init - min_) / range_;
      cur_val_ = init_val_;
    
    }

    void SetRange(float min, float max)
    {
      min_ = min;
      max_ = max;
      range_ = max - min;
    }

    // val is the user value of the param ie, between min and max
    void Lock(float val)
    {
      locked_ = true;
      midi_locked_ = true;
      cur_val_ = (val - min_) / range_;
    }

    // val is the user value of the param ie, between min and max
    void Set(float val)
    {
      locked_ = true;
      cur_val_ = (val - min_) / range_;
    }


    // val is a raw value of the param i.e. between 0 and 1
    void RawLock(float val)
    {
      locked_ = true;
      midi_locked_ = true;
      cur_val_ = fminf(1.0f, fmaxf(0.0f, val));
    }

    // val is a raw value of the param i.e. between 0 and 1
    void RawSet(float val)
    {
      locked_ = true;
      cur_val_ = fminf(1.0f, fmaxf(0.0f, val));
    }

    float Process(float in, uint8_t current_page)
    {
      if (current_page == page_) {
        if (locked_) {
          if (abs(in - cur_val_) < thresh_) {
	    changed_ = true;
	    locked_ = false;
	    midi_locked_ = true;
    	    cur_val_ = in;
          }
        } else {
	  if (in != cur_val_) { 
	    changed_ = true;
	    cur_val_ = in;
	  } else {
	    changed_ = false;
	  }
        }
      } else {
	changed_ = false;
        locked_ = true;
      }
    
      return fminf(max_, fmaxf(min_, (min_ + cur_val_ * range_)));
    }

    inline bool Changed() 
    {
      return changed_;
    }

    /*
     * Midi input will change the current value once it gets within the threshold, regardless of the page
     * It will then lock the value to prevent the physical controls from overriding the midi message.
     * 
     */
    void MidiCCIn(uint8_t midi_byte)
    {
      float in = CC_TO_VAL(midi_byte, 0.0f, 1.0f);
      if (midi_locked_) {
	if (abs(in - cur_val_) < thresh_) {
	  midi_locked_ = false;
	  locked_ = true;
	  changed_ = true;
	  cur_val_ = in;
      	}
      } else {
	if (in != cur_val_) { 
	  changed_ = true;
	  cur_val_ = in;
	} else {
	  changed_ = false;
	}
      }
    }

    /*
     * As above but for we ignore midi locks
     */
    void MidiPBIn(int16_t midi_data)
    {
      float in = PB_TO_VAL(midi_data);
      changed_ = true;
      locked_ = true;
      if (in < 0) {
	cur_val_ = init_val_ + (in * init_val_);
      } else {
	cur_val_ = init_val_ + (in * (1.0 - init_val_));
      }
    }

    float CurVal()
    {
      return cur_val_;
    }

  private:
    bool locked_, midi_locked_, changed_;
    float thresh_, init_val_, cur_val_, min_, max_, range_;
    uint8_t page_;

};
