#pragma once

#define SPM 60.0f

#ifdef TARGET_POD
#include "pod.h"
#endif

#ifdef TARGET_BLUEMCHEN
#include "bluemchen.h"
#endif


using namespace daisy;

template <typename T>
class MidiMsgHandler
{
  public:
    MidiMsgHandler() {}
    ~MidiMsgHandler() {}

    typedef void (*SystemRealTimeHandler)(void);

    typedef void (*MidiMsgHandlerCB)(daisy::MidiEvent *m);

    typedef void (*MidiCCHandlerCB)(uint8_t cc, uint8_t val);

    typedef void (*MidiNOnHandlerCB)(uint8_t n, uint8_t vel);

    typedef void (*MidiNOffHandlerCB)(uint8_t n, uint8_t vel);

    typedef void (*MidiPBHandlerCB)(int16_t val);

    void SetMNOnHCB(MidiNOnHandlerCB cb)
    {
      midi_non_cb_= cb;
    }

    void SetMNOffHCB(MidiNOffHandlerCB cb)
    {
      midi_noff_cb_= cb;
    }

    void SetMPBHCB(MidiPBHandlerCB cb)
    {
      midi_pb_cb_= cb;
    }

    void SetMCCHCB(MidiCCHandlerCB cb)
    {
      midi_cc_cb_= cb;
    }

    void SetMMHCB(MidiMsgHandlerCB cb)
    {
      midi_cb_= cb;
    }

    enum RealTimeType 
    {
      Start,
      Stop,
      Continue,
      Beat,
      HalfBeat
    };

    void SetSRTCB(RealTimeType type, SystemRealTimeHandler cb)
    {
      switch(type) {
        case Start:
          start_cb_ = cb;
          break;
        case Continue:
          cont_cb_ = cb;
          break;
        case Stop:
          stop_cb_ = cb;
          break;
        case Beat:
          beat_cb_ = cb;
          break;
        case HalfBeat:
          half_beat_cb_ = cb;
          break;
        default: break;
      }
    }

    void SetHWHandle(T *hw)
    {
      hw_handle_ = hw;
      tick_dur_ = 1.0f / (2 * hw_handle_->seed.system.GetPClk1Freq());
    }

    void SetChannel(int c)
    {
      channel_ = c;
    }

    void SetPPQN(int ppqn)
    {
      ppqn_ = ppqn;
    }

    float GetBPM()
    {
      return midi_bpm_;
    }

    bool GotClock()
    {
      return got_clock_;
    }

    void ResetGotClock()
    {
      got_clock_ = false;
    }

    void Process()
    {
      hw_handle_->midi.Listen();
      while(hw_handle_->midi.HasEvents())
      {
        daisy::MidiEvent m = hw_handle_->midi.PopEvent();
        if (m.channel != channel_) { return; } 
        switch(m.type) { 
          case daisy::SystemRealTime:
            switch(m.srt_type) {
              case daisy::Start:
		if (start_cb_ != nullptr) {
		  start_cb_();
		}
                ppqn_count_ = 0;
                last_midi_tick_ = hw_handle_->seed.system.GetTick();
                break;
              case daisy::Continue:
		if (cont_cb_ != nullptr) {
		  cont_cb_();
		}
                last_midi_tick_ = hw_handle_->seed.system.GetTick();
                break;
              case daisy::Stop:
		if (stop_cb_ != nullptr) {
		  stop_cb_();
		}
                break;
              case daisy::TimingClock:
                this_midi_tick_ = hw_handle_->seed.system.GetTick();
                if (this_midi_tick_ < last_midi_tick_) {
                  //wrap around?
                  midi_tick_diff_ = (UINT32_MAX - last_midi_tick_) + this_midi_tick_;
                } else {
                  midi_tick_diff_ = this_midi_tick_ - last_midi_tick_;
                }
                last_midi_tick_ = this_midi_tick_;

                avg_midi_diff_ = cum_avg(midi_tick_diff_, ppqn_count_, avg_midi_diff_);
                midi_bpm_ = SPM / (ppqn_ * avg_midi_diff_ * tick_dur_);
                got_clock_ = true;

                if (++ppqn_count_ == ppqn_) {
                  ppqn_count_ = 0;
		  if (beat_cb_ != nullptr) {
		    beat_cb_();
		  }
                }
                if (ppqn_count_ == ppqn_ / 2) {
		  if (half_beat_cb_ != nullptr) {
		    half_beat_cb_();
		  }
                }
                break;
              default: break;
            }
            break;
	  case daisy::NoteOn:
    	  {
	    daisy::NoteOnEvent n = m.AsNoteOn();
	    if (midi_non_cb_ != nullptr) {
	      midi_non_cb_(n.note, n.velocity);
	    }
	    break;
	  }
	  case daisy::NoteOff:
    	  {
	    daisy::NoteOffEvent n = m.AsNoteOff();
	    if (midi_noff_cb_ != nullptr) {
	      midi_noff_cb_(n.note, n.velocity);
	    }
	    break;
	  }
          case daisy::ControlChange:
          {
            daisy::ControlChangeEvent p = m.AsControlChange();
	    if (midi_cc_cb_ != nullptr) {
	      midi_cc_cb_(p.control_number, p.value);
	    }
            break;
          }
	  case daisy::PitchBend:
    	  {
	    daisy::PitchBendEvent p = m.AsPitchBend();
	    if (midi_pb_cb_ != nullptr) {
	      midi_pb_cb_(p.value);
	    }
    	    break;
    	  }
          default:
	    if (midi_cb_ != nullptr) {
	      midi_cb_(&m);
	    }
            break;
        }
      }
    }

  private:
    T *hw_handle_;
    int ppqn_ = 24;
    int channel_ = 0;
    int ppqn_count_ = 0;
    bool got_clock_ = false;
    uint32_t this_midi_tick_ = 0;
    uint32_t last_midi_tick_ = 0;
    uint32_t midi_tick_diff_;
    float midi_bpm_, avg_midi_diff_, tick_dur_;

    SystemRealTimeHandler start_cb_ =	  nullptr;
    SystemRealTimeHandler cont_cb_ =	  nullptr;
    SystemRealTimeHandler stop_cb_ = 	  nullptr;
    SystemRealTimeHandler beat_cb_ = 	  nullptr;
    SystemRealTimeHandler half_beat_cb_ = nullptr;
    MidiCCHandlerCB midi_cc_cb_ = nullptr;
    MidiNOnHandlerCB midi_non_cb_ = nullptr;
    MidiNOffHandlerCB midi_noff_cb_ = nullptr;
    MidiPBHandlerCB midi_pb_cb_ = nullptr;
    MidiMsgHandlerCB midi_cb_ =	  nullptr;

    // n starts from zero
    float cum_avg(uint32_t x, int n, float ca)
    {
      return (float)((x + (n * ca)) / (n + 1));
    }
};
