# Tesladuino
Code for a custom Singing Tesla Coil Interrupter based on the Arduino platform.

## Desired Feature List - *italics are future features*
* Control of OneTesla SSTCs over optical-fibre
  * Manual Control (Frequency and Volume) Similar to OneTesla Fixed Mode
  * MIDI File Playback from SD Card
  * *MIDI In for live playback*
* Battery powered from internal rechargeable battery
* Self-contained (No PC Required to run)

## Components - What I used but any mix will work with code modifications.
* Adafruit Feather m0 Adalogger (built in SD & more powerful SAMD21 Processor. Equiv to Arduino/Genuino MKR Zero)
* SSD 1306 Monochrome 0.96" 128x64 OLED Display
* Rotary Encoder /w Click (Volume Knob) & Knob
* 600mAh LiPo battery
* Power rocker switch & push buttons for interface
* Handheld Enclosure (Hammond Manufacturing)
* Micro USB Panel Mount Extension
* TOSLINK connector Optical out (replace coil receiver with TOSLINK too?)
* *MIDI In jack & Optoisolator*

## Libraries used
* Adafruit_SSD1306 & Adafruit-GFX-Library
* Arduino Default SD Library
* Rotary Encoder Library (TBC)
* MIDI Library (TBC)
