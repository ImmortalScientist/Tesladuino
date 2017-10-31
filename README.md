# Tesladuino
Code for a custom Singing Tesla Coil Interrupter based on the Arduino platform (software).

## Desired Feature List - *italics are future features*
* Battery powered from internal rechargeable battery
* Self-contained (No PC Required to run)
* Control of OneTesla SSTCs over optical-fibre
  * Manual Control (Frequency and Volume) Similar to OneTesla Fixed Mode
  * MIDI File Playback from SD Card
  * *MIDI In for live playback*

## Output Signal Specification
* Pulse Width (Volume) - 6.5us to 47.5us
* Frequency - Matches desired sound frequency
* Maximum total duty-cycle (i.e. max avg. output power) =

## Components - What I used but any mix will work with code modifications.
* Adafruit Feather M0 Adalogger (built in SD & more powerful SAMD21 Processor. Equiv to Arduino/Genuino MKR Zero)
* SSD 1306 Monochrome 0.96" 128x64 OLED Display
* Rotary Encoder /w Click (Volume Knob) & Knob
* 600mAh LiPo battery
* Power rocker switch & push buttons for interface
* Handheld Enclosure (Hammond Manufacturing)
* Micro USB Panel Mount Extension
* TOSLINK connector Optical out (replace coil receiver with TOSLINK too?)
* *MIDI In jack & Optoisolator*

## Dependencies
* Adafruit_SSD1306 & Adafruit-GFX-Library
* Arduino Default SD Library
* Rotary Encoder Library (TBC)
* MIDI Library (TBC)
