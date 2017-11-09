#include "inc_encoder.h"
#include "OLED.h"
#include "data.h"
#include "constants.h"

extern volatile int ffreq;
extern volatile char volindex;
extern volatile char mode;

extern volatile char encNEXT;
extern volatile char encPREV;

extern volatile char enc_pos_changed;

void encoder_init(void)
{  
  pinMode(ENCODER_A, INPUT_PULLUP); 
  pinMode(ENCODER_B, INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoder_int_handler, FALLING);
}

void encoder_int_handler(void)
{
  /* If ENCODER_A and ENCODER_B are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   */

  // only process an encoder change if we have dealt with the last one already
  if (!enc_pos_changed)
  {
    if (digitalRead(ENCODER_A) == digitalRead(ENCODER_B))
    {
      // if we are in freq adjust mode, decrease frequency
      if (mode == ADJ_MODE_FREQ)
      {
        ffreq--;
        if (ffreq <= MIN_FREQ)
        {
          ffreq = MIN_FREQ;
        }     
      }
      // else, decrease volume
      else if (mode == ADJ_MODE_VOL)
      {
        volindex--;
        if (volindex <= 0)
        {
          volindex = 0; 
        }
      }
      else if (mode = ADJ_MODE_FILE)
      {
        encPREV = 1;
        encNEXT = 0;
      }
      else if (mode == ADJ_MODE_MENU)
      {
        encPREV = 1;
        encNEXT = 0;
      }
    }
    else
    {
      // if we are in freq adjust mode, increase frequency
      if (mode == ADJ_MODE_FREQ)
      {
          ffreq++;
          if (ffreq >= MAX_FREQ)
          {
            ffreq = MAX_FREQ;
          }
      }
      // else, increase volume
      else if (mode == ADJ_MODE_VOL)
      {
        volindex++;
        if (volindex >= 60)
        {
          volindex = 60; 
        }
      }
      else if (mode = ADJ_MODE_FILE)
      {
        encPREV = 0;
        encNEXT = 1;
      }
      else if (mode == ADJ_MODE_MENU)
      {
        encPREV = 0;
        encNEXT = 1;
      }
    }
  // signal that something has been updated
  enc_pos_changed = 1;
  }
}
