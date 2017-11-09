#include "serialsource.h"
#include "datatypes.h"
#include "data.h"
#include "player.h"
#include "util.h"
#include "constants.h"
#include "system.h"

#include <stdlib.h>
#include <avr/io.h>
#include <Arduino.h>

#include "OLED.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

extern SSD1306AsciiWire oled;
extern volatile char enc_pos_changed;
extern volatile char mode;

void USART_init(void)
{
 UBRR0H = (uint8_t) (BAUD_PRESCALER >> 8);
 UBRR0L = (uint8_t) (BAUD_PRESCALER);
 UCSR0B = (1 << RXEN0) | (1 << TXEN0);
 UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));
}

void serialsource_init()
{
  USART_init();
  serial = (serialsource*) malloc(sizeof(serialsource));
  serial->last_status = 0;
  serial->buf = (unsigned char*) malloc(2 * sizeof(char));
  last_message = (midiMsg*) malloc(sizeof(midiMsg));
}

void serialsource_run()
{
  mode = ADJ_MODE_VOL;
  draw_midi_volbar();
  note1->pitch = note2->pitch = 0;
  unsigned char b;
  unsigned char cnt = 0;
  unsigned long lt = millis();
  for (;;)
  {
    unsigned long t = millis();
    if (enc_pos_changed)
    {
      draw_midi_volbar();
      enc_pos_changed = 0;
    }
    if (t - lt > 150)
    {
      lt = t;
      unsigned char k = get_key();
      if ((k == btnSELECT) || (k == btnRETURN))
      {
        note_stop();
        return;
      }
      else if (k == btnUP)
      {
        //incvol(&lcd);
        volindex++;
        draw_midi_volbar();
        lt = t;
      }
      else if (k == btnDOWN)
      {
        //decvol(&lcd);
        volindex--;
        draw_midi_volbar();
        lt = t;
      }
    }
    if (!(UCSR0A & (1 << RXC0))) continue;
    b = UDR0;

    if ((b & 0x80) >> 7 > 0)
    {
      unsigned char msn = b & 0xf0;
      if (msn != 0x80 && msn != 0x90 && msn != 0xb0 & msn != 0xe0)
      {
        serial->last_status = 0;
      }
      else
      {
        serial->last_status = b;
        serial->remaining = 2;
        serial->total = 2;
        serial->buf_idx = 0;
      }
    }
    
    if (serial->last_status == 0) continue;
    
    if ((b & 0x80) >> 7 == 0)
    {
      serial->buf[serial->buf_idx] = b;
      serial->buf_idx++;
      serial->remaining--;
    }
    
    if (serial->remaining == 0)
    {
      last_message->cmd = serial->last_status;
      last_message->db1 = serial->buf[0];
      last_message->db2 = serial->buf[1];
      serial->remaining = serial->total;
      serial->buf_idx = 0;
      parsemsg(last_message);
    }
  }
}
