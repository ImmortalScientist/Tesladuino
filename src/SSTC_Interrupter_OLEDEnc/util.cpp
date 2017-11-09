#include "datatypes.h"
#include "data.h"
#include "util.h"
#include "constants.h"
#include <Arduino.h>
#include <SdFat.h>

void note_kill(note *n)
{
  n->pitch = 0;
}

void make_note(note *n, unsigned char pitch, unsigned char velocity)
{
  n->pitch = pitch;
  unsigned long freq = get_freq(pitch);
  n->period = 1000000 / freq;
  n->on_time = get_on_time(freq);
  n->velocity = map_velocity(velocity);
}

unsigned long get_freq(unsigned char pitch)
{
  int n = pitch - 57;
  if (n >= 0)
  {
    return pow2[n / 12] * base[n % 12];
  }
  else
  {
    int a = (-(n+ 1)) / 12 + 1;
    int r = 12 * a + n;
    return base[r] / pow2[a];
  }
}

unsigned int get_on_time(long freq) {
  int index = freq / 100;
  if (index > ON_TIME_ARRAY_LENGTH - 1)
  {
    return 0;
  }
  else
  {
    return LOOKUP_TABLE_SCALE * on_times[index];
  }
}

unsigned char get_key()
{
  //if (!digitalRead(BTN_UP) && !digitalRead(BTN_DN)) return btnRETURN;
  if (!digitalRead(BTN_RTN)) return btnRETURN;  
  if (!digitalRead(BTN_UP)) return btnUP;
  if (!digitalRead(BTN_DN)) return btnDOWN;
  if (!digitalRead(BTN_OK)) return btnSELECT;
  
  return btnNONE;
}
