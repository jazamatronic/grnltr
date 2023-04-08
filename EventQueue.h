#pragma once

#define QUEUE_LENGTH 16

template <size_t queue_length>
class EventQueue
{
  public:

    EventQueue() : qhead_(0), qtail_(0), qfull_(false) {}
    ~EventQueue() {}

  enum event {
    PAGE_UP,
    PAGE_DN,
    INCR_GRAIN_ENV,
    RST_PITCH_SCAN,
    TOG_GRAIN_REV,
    TOG_SCAN_REV,
    TOG_SCAT,
    TOG_FREEZE,
    TOG_RND_PITCH,
    TOG_RND_DENS,
    INCR_WAV,
    TOG_LOOP,
    LIVE_REC,
    LIVE_PLAY,
    INCR_MIDI,
    NEXT_DIR,
    NONE
  };

  struct event_entry {
    event ev;
    uint8_t id;
  };

  void push_event(event ev, uint8_t id)
  {
    if (qfull_) { 
      //drop it like its hot
      return;
    }
    event_queue_[qtail_].ev = ev;
    event_queue_[qtail_].id = id;
    qtail_ = (qtail_ + 1) % queue_length;
    if (qtail_ == qhead_) {
      qfull_ = true;
    }
  }

  event_entry pull_event()
  {
    event_entry this_event;
    this_event = event_queue_[qhead_];
    qhead_ = (qhead_ + 1) % queue_length;
    if (qfull_) {
      qfull_ = false;
    }
    return this_event;
  }

  bool has_event()
  {
    return (qfull_ || !(qtail_ == qhead_));
  }

  private:
    event_entry event_queue_[queue_length];
    uint8_t qhead_;
    uint8_t qtail_;
    bool qfull_;
};
