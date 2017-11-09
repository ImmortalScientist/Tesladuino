/*
 * ATMega328 SD card interrupter
 * Copyright 2015 oneTesla, LLC
 * See README.txt for licensing information
 */
 
#include "constants.h"
#include "data.h"
#include "datatypes.h"
#include "sdsource.h"
#include "serialsource.h"
#include "player.h"
#include "system.h"
#include "util.h"
#include "timers.h"
#include <SdFat.h>

#include "OLED.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "inc_encoder.h"

#define I2C_ADDRESS 0x78
#define OPTMIZE_I2C

note *note1, *note2;
midiMsg *last_message;
serialsource *serial;
sdsource *sd;

volatile char volindex = 0;
volatile char menuindex = 0;
volatile int ffreq = MIN_FREQ;
volatile char mode = ADJ_MODE_NONE;

volatile char encNEXT = 0;
volatile char encPREV = 0;
volatile char enc_pos_changed = 0;

void fixedLoop();
void displayMenu();

SSD1306AsciiWire oled;

void setup()
{
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  TWBR = 0;   // set the i2c to 1MHz
  oled.clear();
  
  oled.setFont(Arial_bold_14);
  oled.setCursor(10, OLED_LINE1);
  oled.print("Tesla Controller");
  
  oled.setFont(Arial14);
  oled.setCursor(30, OLED_LINE3);
  oled.print("Booting...");
  
  setupTimers();
  setupPins();
  player_init();
  sdsource_init();
  serialsource_init();
  
  encoder_init();

  delay(500);

  oled.setCursor(20, OLED_LINE3);
  oled.clearToEOL();
 
  displayMenu();
}

void loop()
{  
  char mode = ADJ_MODE_MENU;
  unsigned char key = get_key();
  if ((key == btnDOWN) || encPREV)
  {
    if (encPREV)
    {
      encNEXT = 0;
      encPREV = 0;
      enc_pos_changed = 0;
    }
    if (menuindex == 2)
    {
      menuindex = 0;
    }
    else
    {
      menuindex++;
    }
    displayMenu();
    delay(180);
  }
  if ((key == btnUP) || encNEXT)
  {
    if (encNEXT)
    {
      encNEXT = 0;
      encPREV = 0;
      enc_pos_changed = 0;
    }
    if (menuindex == 0)
    {
      menuindex = 2;
    }
    else
    {
      menuindex--;
    }
    displayMenu();
    delay(180);
  }
  if (key == btnSELECT)
  {
    if (menuindex == MENU_FIXED)
    {
      delay(150);
      fixedLoop();
      delay(300);
    }
    else if (menuindex == MENU_LIVE)
    {
      delay(150);
      oled.setCursor(0, OLED_LINE2);
      oled.clearToEOL();
      oled.print("Live Mode: ON");
      serialsource_run();
      //lcd_printhome("Live Mode");
      delay(300);
    }
    else
    { // MENU_SDCARD
      delay(150);
      if (sd->valid)
      {
        sdsource_run();
      }
      else
      {
        sdsource_initcard();
      }
      delay(300);
    }
    displayMenu();
    while (get_key() != btnNONE) {};
  }
}

void displayMenu()
{  
  if (menuindex == MENU_SDCARD)
  {
    oled.setCursor(0, OLED_LINE3);
    oled.clearToEOL();
    oled.setCursor(0, OLED_LINE2);
    oled.clearToEOL();
    oled.print("SD MIDI Mode: OFF");
    oled.setCursor(0, OLED_LINE3);
    oled.clearToEOL();
    if (sd->valid)
    {          
      //sprintf(oled_line, "%d File(s)", sd->file_count);
      //oled.write(oled_line);
      oled.print("SD Card: ");
      oled.print(sd->file_count);
      oled.print(" File(s)");
    }
    else
    {
      //sprintf(oled_line, "%s", sd->last_error);
      //oled.write(oled_line);
      oled.print("SD Error! ");
      oled.print(sd->last_error);
    }
  }
  else if (menuindex == MENU_LIVE)
  {
    oled.setCursor(0, OLED_LINE3);
    oled.clearToEOL();
    oled.setCursor(0, OLED_LINE2);
    oled.clearToEOL();    
    oled.print("Live Mode: OFF");    
  }
  else
  {
    oled.setCursor(0, OLED_LINE3);
    oled.clearToEOL();
    oled.setCursor(0, OLED_LINE2);
    oled.clearToEOL();
    oled.print("Fixed Mode: OFF");    
  }
  oled.setCursor(0, OLED_LINE4);
  oled.clearToEOL();
}

void fixedLoop()
{
  mode = ADJ_MODE_FREQ;
  unsigned long elapsed = 0;
  unsigned int uinc, dinc = 1;

  oled.setCursor(0, OLED_LINE2);
  oled.clearToEOL();
  oled.print("Fixed Mode: ON");    
  
  draw_fixed_screen();
  note1->velocity = 127;
  note1->on_time = get_on_time(ffreq);
  setTimer1f(ffreq);  
  engageISR1();
  
  for (;;)
  {
    unsigned char key = get_key();
    if (enc_pos_changed)
    {
      draw_fixed_screen();
      enc_pos_changed = 0;
      if (mode == ADJ_MODE_FREQ)
      {
        note1->on_time = get_on_time(ffreq);
        setTimer1f(ffreq);
      }
    }
    if (elapsed > 100000)
    {
      elapsed = 0;
      if (key != btnUP)
      {
        uinc = 1;
      }
      if (key != btnDOWN)
      {
        dinc = 1;
      }
      if (key == btnSELECT)
      {
        if (mode == ADJ_MODE_FREQ)
        {
          mode = ADJ_MODE_VOL;
        }
        else if (mode == ADJ_MODE_VOL)
        {
          mode = ADJ_MODE_FREQ;
        }
        draw_fixed_screen();
      }
      if (key == btnUP)
      {
        if (mode == ADJ_MODE_VOL)
        {
          volindex++;
        }
        else if (mode = ADJ_MODE_FREQ)
        {
          ffreq += uinc;
          uinc += 5;
          if (ffreq > MAX_FREQ)
          {
            ffreq = MAX_FREQ;
          }

          note1->on_time = get_on_time(ffreq);
          setTimer1f(ffreq);
        }
        draw_fixed_screen();
      }
      if (key == btnDOWN)
      {
        if (mode == ADJ_MODE_VOL)
        {
          volindex--;
        }
        else if (mode = ADJ_MODE_FREQ)
        {
          ffreq -= dinc;
          dinc += 5;
          if (ffreq < MIN_FREQ)
          {
            ffreq = MIN_FREQ;
          }
          note1->on_time = get_on_time(ffreq);
          setTimer1f(ffreq);;
        }
        draw_fixed_screen();
      }
      if (key == btnRETURN)
      {
        note_stop();
        return;
      }
    }
    delayMicroseconds(1000);
    elapsed += 1000;
  }
}

int main()
{
  init();
  setup();
  for(;;)
  {
    loop();
  }
  return 0;
}

