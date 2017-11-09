# Tesladuino
Code for a custom Singing Tesla Coil Interrupter based on the Arduino platform
(software). Included here are the Interrupter Firmware packages from OneTesla
LLC which I intend to base sections of in this project.

# Specification
## Desired Feature List
* Battery powered from internal rechargeable battery
* Self-contained (No PC Required to run)
* Control of OneTesla SSTCs over optical-fibre
  * Manual Control (Frequency and Volume) Similar to OneTesla Fixed Mode
  * MIDI File Playback from SD Card
  * *MIDI In for live playback - Future Version*

# Implementation
## Hardware - What I used but any mix will work with code modifications.
* Adafruit Feather M0 Adalogger (built in SD & more powerful SAMD21 Processor.
  Equivalent to Arduino/Genuino MKR Zero)
* SSD 1306 Monochrome 0.96" 128x64 OLED Display
* Rotary Encoder /w Click (Volume Knob) & Knob
* 600mAh LiPo battery
* Power rocker switch & push buttons for interface
* Handheld Enclosure (Hammond Manufacturing)
* Micro USB Panel Mount Extension
* TOSLINK connector Optical out (replace coil receiver with TOSLINK too?)
* *MIDI In jack & Optoisolator*

## Software
### Menu
* 1 Button (Back/Esc) & click encoder (Select).
* Menu Tree:
  * **Fixed Mode**
    * Volume Adjust
    * Frequency Adjust
  * SD Card
