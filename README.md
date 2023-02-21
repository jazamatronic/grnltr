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

Parameters are paged and the current page is indicated by the colour of LED1. Turn the encoder to change pages.  

k1 = knob1, b1 = button1 etc.  

RED = Pitch/Time page  
&nbsp;&nbsp;k1 = Grain Pitch -2 8va to +2 8va  
&nbsp;&nbsp;k2 = Scan Rate  
&nbsp;&nbsp;b1 = cycle env type  
&nbsp;&nbsp;b2 = Reset Grain Pitch and Scan Rate  
        
ORANGE = Grain Duration/Density page  
&nbsp;&nbsp;k1 = Grain Duration 10mS to 200mS  
&nbsp;&nbsp;k2 = Grain Density  2 per second to 200 per second  
&nbsp;&nbsp;b1 = Grain Reverse  
&nbsp;&nbsp;b2 = Scan Reverse  
        
YELLOW = Grain Scatter page  
&nbsp;&nbsp;k1 = Scatter Distance  
&nbsp;&nbsp;b1 = Toggle Scatter  
&nbsp;&nbsp;b2 = Toggle Freeze  
        
GREEN = Random Grain Pitch page  
&nbsp;&nbsp;k1 = Pitch Distance  
&nbsp;&nbsp;b1 = Toggle Random Pitch  
        
BLUE = WAV selection page  
&nbsp;&nbsp;NOTE: led flashes blue during start up to indicate file reading  
&nbsp;&nbsp;k1 = Sample Start  
&nbsp;&nbsp;k2 = Sample End  
&nbsp;&nbsp;b1 = Cycle Wave  
&nbsp;&nbsp;b2 = Toggle Wave Loop  
        
PURPLE = Decimator page  
&nbsp;&nbsp;k1 = Bit Crush  
&nbsp;&nbsp;k2 = Downsample  
        
## TODO  
Add recording capability  
Add MIDI control  
Add Directory Browsing - Need a screen - target kxmx_bluemchen   
Provide a Demo  
  
A demo and feature walkthrough of the v0.7 version available click here:  
[![grnltr_demo](https://img.youtube.com/vi/RLfN7tFsF2Q/0.jpg)](https://youtu.be/RLfN7tFsF2Q "grnltr demo")
