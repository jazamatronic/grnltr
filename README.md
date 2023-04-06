# grnltr  

## Author

<!-- Insert Your Name Here -->
Jared ANDERSON

20230220

## Description

grnltr - a Granulator

Initial build for the Daisy Seed platform running on the pod 
Now supports Bluemchen as a build target.  

Default make will build for the pod, to build for bluemchen use:  

```
$ make BUILD_TARGET=bluemchen
$ make BUILD_TARGET=bluemchen program-dfu
```

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
Pressing the encoder will cycle the current sample.  This can also be done by seinding MIDI NoteOn - From Note 60 (C3)  
MIDI parameters are accepted no matter what page is currently active.  
Knobs and MIDI CC messages are in "catch" mode.  
For Toggle parameters, send any CC value to toggle.

| LED Colour | Page | Knob1 | Knob2 | Button1 | Button2 |
| ---------- | ---- | ----- | ----- | ------- | ------- |
| RED | Pitch/Time | Grain Pitch (-2 8va to +2 8va)<br> CC3 and Pitch Bend | Scan Rate\*<br> CC1 | Cycle env type | Reset Grain Pitch and Scan Rate |
| ORANGE | Grain Duration/Density | Grain Duration (10 to 200mS)<br> CC9 | Grain Density (2 to 200 per second)<br> CC14 | Grain Reverse<br> CC25 | Scan Reverse\*<br> CC26 |
| YELLOW | Grain Scatter | Scatter Distance<br> CC20 | N/A | Toggle Scatter<br> CC15 | Toggle Freeze<br> CC27 |
| GREEN | Randomize | Pitch Distance<br> CC22 | N/A | Toggle Random Pitch<br> CC21 | Toggle Random Density<br> CC29 |
| BLUE | WAV Select | Sample Start\*<br> CC12 Coarse, CC44 Fine | Sample End\*<br> CC13 Coarse, CC45 Fine | Live Rec Mode<br> CC31 | Play Rec Buffer<br> CC32 |
| PURPLE | Decimate/Record | Bit Crush<br> CC23 | Downsample<br> CC24 | Toggle Wave Loop\*<br> CC28 | N/A |

Parameters marked with a \* are disabled in live record mode.

On the bluemchen the two knobs work as for the pod.  
To emulate the buttons, long press the encoder to access parameter select mode.  
Select a parameter with the encoder, short press to activate it.  
Long press again to exit parameter select mode.  
  
A demo and feature walkthrough of the v0.7 version available, click here:  
[![grnltr_demo](https://img.youtube.com/vi/RLfN7tFsF2Q/0.jpg)](https://youtu.be/RLfN7tFsF2Q "grnltr demo")  

## TODO  
Add recording capability: this is somewhat addressed in a passthrough mode. Recording a set number of bars and then writing to SD is not yet implemented.    
~~Add MIDI control~~    
Add Directory Browsing - Need a screen - target kxmx_bluemchen   
~~Provide a Demo~~  
  
