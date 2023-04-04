# grnltr  

## Author

<!-- Insert Your Name Here -->
Jared ANDERSON

20230220

## Description

grnltr - a Granulator

Initial build for the Daisy Seed platform running on the pod 

Clone this under the DaisyExamples/pod directory and run make to build.  

In its current form, it reads up to 16 WAV files (total sample size must be < 64MB) from an SDMMC card that can then be granulated.
Waves must be in mono s16 format.  I use sox to do conversion - something like:  

```
# convert to 16bit PCM 1 channel 48k wave
sox <in.wav> -r 48000 -c 1 -b 16 -e s <out.wav>
```

There is now also a small record buffer for "live" pass through granulization.  
Once engaged, the buffer will fill once before grain processing starts.  
Some parameters are disabled in this mode.

Parameters are paged and the current page is indicated by the colour of LED1. Turn the encoder to change pages.  
MIDI parameters are accepted no matter what page is currently active.  
Knobs and MIDI CC messages are in "catch" mode.  
For Toggle parameters, send any CC value to toggle.

k1 = knob1, b1 = button1 etc.  

RED = Pitch/Time page  
&nbsp;&nbsp;k1 = Grain Pitch -2 8va to +2 8va  
&nbsp;&nbsp;&nbsp;&nbsp;CC3 and Pitch Bend  
&nbsp;&nbsp;k2 = Scan Rate\*  
&nbsp;&nbsp;&nbsp;&nbsp;CC1  
&nbsp;&nbsp;b1 = cycle env type  
&nbsp;&nbsp;b2 = Reset Grain Pitch and Scan Rate  
        
ORANGE = Grain Duration/Density page  
&nbsp;&nbsp;k1 = Grain Duration 10mS to 200mS  
&nbsp;&nbsp;&nbsp;&nbsp;CC9  
&nbsp;&nbsp;k2 = Grain Density  2 per second to 200 per second  
&nbsp;&nbsp;&nbsp;&nbsp;CC14  
&nbsp;&nbsp;b1 = Grain Reverse  
&nbsp;&nbsp;&nbsp;&nbsp;CC25  
&nbsp;&nbsp;b2 = Scan Reverse\*  
&nbsp;&nbsp;&nbsp;&nbsp;CC26  
        
YELLOW = Grain Scatter page  
&nbsp;&nbsp;k1 = Scatter Distance  
&nbsp;&nbsp;&nbsp;&nbsp;CC20  
&nbsp;&nbsp;b1 = Toggle Scatter  
&nbsp;&nbsp;&nbsp;&nbsp;CC15  
&nbsp;&nbsp;b2 = Toggle Freeze  
&nbsp;&nbsp;&nbsp;&nbsp;CC27  
        
GREEN = Random Grain Pitch page  
&nbsp;&nbsp;k1 = Pitch Distance  
&nbsp;&nbsp;&nbsp;&nbsp;CC22  
&nbsp;&nbsp;b1 = Toggle Random Pitch  
&nbsp;&nbsp;&nbsp;&nbsp;CC21  
        
BLUE = WAV selection page  
&nbsp;&nbsp;NOTE: led flashes blue during start up to indicate file reading  
&nbsp;&nbsp;k1 = Sample Start\*  
&nbsp;&nbsp;&nbsp;&nbsp;CC12 - Coarse  
&nbsp;&nbsp;&nbsp;&nbsp;CC44 - Fine  
&nbsp;&nbsp;k2 = Sample End\*  
&nbsp;&nbsp;&nbsp;&nbsp;CC13 - Coarse  
&nbsp;&nbsp;&nbsp;&nbsp;CC45 - Fine  
&nbsp;&nbsp;b1 = Cycle Wave  
&nbsp;&nbsp;&nbsp;&nbsp;NoteOn - Note 60 (C3) upwards  
&nbsp;&nbsp;b2 = Toggle Wave Loop\*  
&nbsp;&nbsp;&nbsp;&nbsp;CC28  
        
PURPLE = Decimator page  
&nbsp;&nbsp;k1 = Bit Crush  
&nbsp;&nbsp;&nbsp;&nbsp;CC23  
&nbsp;&nbsp;k2 = Downsample  
&nbsp;&nbsp;&nbsp;&nbsp;CC24  
&nbsp;&nbsp;b1 = Live Rec Mode  
&nbsp;&nbsp;&nbsp;&nbsp;CC31  
&nbsp;&nbsp;b2 = Play Rec Buffer  
&nbsp;&nbsp;&nbsp;&nbsp;CC32  

Parameters marked with a \* are disabled in live record mode.
        
A demo and feature walkthrough of the v0.7 version available, click here:  
[![grnltr_demo](https://img.youtube.com/vi/RLfN7tFsF2Q/0.jpg)](https://youtu.be/RLfN7tFsF2Q "grnltr demo")  

## TODO  
Add recording capability: this is somewhat addressed in a passthrough mode. Recording a set number of bars and then writing to SD is not yet implemented.    
~~Add MIDI control~~    
Add Directory Browsing - Need a screen - target kxmx_bluemchen   
~~Provide a Demo~~  
  
