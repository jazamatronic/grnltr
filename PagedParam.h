#pragma once

#define CC_TO_VAL(x, min, max) (min + (x / 127.0f) * (max - min))

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
      min_ = min;
      max_ = max;
      range_ = max_ - min_;
    
      cur_val_ = (init - min_) / range_;
    
    }

    void SetRange(float min, float max)
    {
      min_ = min;
      max_ = max;
      range_ = max - min;
    }


    void lock(float in)
    {
      locked_ = true;
      cur_val_ = (in - min_) / range_;
    }

    float Process(float in, uint8_t current_page)
    {
      if (current_page == page_) {
        if (locked_) {
          if (abs(in - cur_val_) < thresh_) {
    	locked_ = false;
    	cur_val_ = in;
          }
        } else {
          cur_val_ = in;
        }
      } else {
        locked_ = true;
      }
    
      return fminf(max_, fmaxf(min_, (min_ + cur_val_ * range_)));
    }

    /*
     * Midi input ignores pages and sets the value immediately.
     * It will always lock the value to prevent the physical controls from overriding
     * the midi message.
     */
    float MidiIn(uint8_t midi_byte)
    {
      locked_ = true;
      cur_val_ = CC_TO_VAL(midi_byte, 0.0f, 1.0f);
      return min_ + cur_val_ * range_;
    }

  private:
    bool locked_;
    float thresh_, cur_val_, min_, max_, range_;
    uint8_t page_;

};
