#include <SdFat.h>
#include <stdlib.h>
#include "constants.h"
#include "sdsource.h"
#include "data.h"
#include "datatypes.h"
#include "player.h"
#include "util.h"

#include "OLED.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

extern SSD1306AsciiWire oled;

extern volatile char enc_pos_changed;
extern volatile char mode;

extern volatile char encNEXT;
extern volatile char encPREV;

SdFat fs;
SdFile file;

char filename[FILENAME_SIZE];

void sdsource_init()
{
  sd = (sdsource*) malloc(sizeof(sdsource));
  sd->buf = (unsigned char*) calloc(FILE_BUFFER_SIZE * sizeof(char), sizeof(char));
  sd->next = (timedMidiMsg*) malloc(sizeof(timedMidiMsg));
  sd->p_msg = (midiMsg*) malloc(sizeof(midiMsg));
  sd->last_error[0] = '\0';
  sd->valid = 0;
  sd->file_count = 0;

  sdsource_initcard();
}

void sdsource_initcard()
{
  oled.setCursor(0, OLED_LINE3);
  oled.clearToEOL();
  oled.print("Checking SD Card...");
  delay(250);
  //lcd_printhome("Checking SD Card");
  if (!fs.begin(MMC_CS, SPI_FULL_SPEED))
  {
    if (fs.card()->errorCode())
    {
      // Can use sd.card()->errorData() to find out more information.
      // errorCode of 0x01; errorData of 0xFF is wiring error or no card.
      strcpy(sd->last_error, "No card found");
      return;
    }
    if (fs.vol()->fatType() == 0)
    {
      strcpy(sd->last_error, "Inv Partition");
      return;
    }
    if (fs.vwd()->isOpen())
    {
      strcpy(sd->last_error, "Inv Root");
      return;
    }
    strcpy(sd->last_error, "Unknown error");
    return;
  }

  while(sdsource_filenext(false))
  {
    sd->file_count++;
  }
  file.close();
  fs.vwd()->rewind();

  if (sd->file_count == 0)
  {
    strcpy(sd->last_error, "No valid files");
  }

  sd->valid = 1;
}

unsigned char sdsource_filenext(bool loadfile = false)
{
  if (file.isOpen())
  {
    file.close();
  }
  while (file.openNext(fs.vwd(), O_READ))
  {
    file.getName(filename, sizeof(filename));
    if (check_and_get_filename())
    {
        if (loadfile) sdsource_loadfile();
        return 1;
    }
    file.close();
  }
  return 0;
}

unsigned char sdsource_fileprev(bool loadfile = false)
{
  // get the current position
  uint16_t index = (fs.vwd()->curPosition()) / 32;
  // make sure we dont be at the end of the directory if we start looking backwards
  if (index < 2)
  {
    return 0;
  }
  // decrement the index by 2 so we have a place to start looking backwards from
  index = index - 2;
  //  if any files are open at this time, close them
  if (file.isOpen())
  {
    file.close();
  } 
  // look backwards until we hit a file of the start of the directory
  while(index > 0)
  {
    // if opening a file is successful, load and display the file
    if (file.open(fs.vwd(), index, O_READ));
    {
      file.getName(filename, sizeof(filename));
      if (check_and_get_filename())
      {
          if (loadfile)
          {
            sdsource_loadfile();
          }
          return 1;
      }
    }
    // keep looking backwards one file at a time
    index--;
  }
  return 0;
}

unsigned char check_and_get_filename()
{
  char *dot = strrchr(filename, '.');
  if (strcasecmp(dot, ".omd") != 0) return 0;
  *dot = '\0'; // Cut off the extension for nicer display
  return 1;
}

void sdsource_showfilename(int pos = 0)
{
  int fn_len = strlen(filename) - 16;
  int start = 0;
  if (fn_len < 0)
  {
    fn_len = 0;
  }
  else
  {
    start = (pos % (fn_len + 6)) - 3;
  }

  // Pause for 3 beats at the beginning and end of the scroll
  if (start < 0)
  {
    start = 0;
  }
  else if (start >= fn_len)
  {
    start = fn_len;
  }

  oled.setCursor(0, OLED_LINE2);
  oled.clearToEOL();
  oled.write(&filename[start]);
  
  //lcd_printhome(&filename[start]);
  //lcd_setcursor(0, 1);

  unsigned int lsec = (int) (sd->len / 1000);

  oled.setCursor(0, OLED_LINE3);
  oled.clearToEOL();
  oled.setCursor(50, OLED_LINE3);
  oled.clearToEOL();
  oled.print(lsec / 60);
  oled.print(":");
    
  //lcd_print(lsec / 60);
  //lcd_print(':');
  unsigned int tmp = lsec % 60;
  if (tmp < 10)
  {
    oled.print("0");
    //lcd_print('0');
  }
  oled.print(tmp);
  //lcd_print(tmp);
}

void sdsource_loadfile()
{
  memset(sd->buf, 0, FILE_BUFFER_SIZE);  
  sd->read_count = file.read(sd->buf, FILE_BUFFER_SIZE);
  sd->len = ((long) sd->buf[0] << 24) + ((long) sd->buf[1] << 16) + ((long) sd->buf[2] << 8) + (long) sd->buf[3];
  sd->buf_index = 4;
  sd->time = 0;
  sd->buf_index = read_msg(sd->buf, sd->buf_index, sd->next);
}

void sdsource_run()
{
  mode = ADJ_MODE_FILE;
  encNEXT = 0;
  encPREV = 0;
  
  unsigned long kt = millis();
  unsigned long old_key_t = millis();
  int filepos = 0;

  draw_midi_volbar();

  sdsource_filenext(true);
  sdsource_showfilename(0);

  unsigned char old_key = btnNONE;
  unsigned char key = btnNONE;

  for (;;)
  {
    unsigned long t = millis();

    if (t - old_key_t > 200)
    {
      old_key = btnNONE;
      old_key_t = t;
    }
    else
    {
      old_key = key;
    }
    key = get_key();

    if (t - kt > 300)
    {
      sdsource_showfilename(filepos++);
      kt = t;
    }

    if (old_key == btnNONE && key == btnSELECT)
    {
      delay(150);
      mode = ADJ_MODE_VOL;
      sdsource_playfile();
      mode = ADJ_MODE_FILE;
      delay(300);
    }

    if ((old_key == btnNONE && key == btnUP) || encNEXT)
    {
      if (encNEXT)
      {
        encNEXT = 0;
        encPREV = 0;
        enc_pos_changed = 0;
      }
      //delay(150);
      if (!sdsource_filenext(true))
      {
        file.close();
        fs.vwd()->rewind();
        sdsource_filenext(true);
      }
      filepos = 0; kt = t;
      //delay(300);
    }

    if ((old_key == btnNONE && key == btnDOWN) || encPREV)
    {
      if (encPREV)
      {
        encNEXT = 0;
        encPREV = 0;
        enc_pos_changed = 0;
      }
      //delay(150);
      sdsource_fileprev(true);
      filepos = 0; kt = t;
      //delay(300);
    }

    if (key == btnRETURN)
    {
      delay(150);
      note_stop();
      return;
    }
  }
}

unsigned int read_msg(unsigned char *track, unsigned int index, timedMidiMsg* msg)
{
  if (index >= sd->read_count)
  {
    sd->read_count = file.read(sd->buf, FILE_BUFFER_SIZE);
    if (sd->read_count == 0)
    {
      return 0;
    }
    index = 0;
  }
  msg->time = ((((unsigned int) track[index]) << 7) + (track[index + 1] & 0x7f));
  msg->cmd = ((track[index + 1] & 0x80) >> 7) ? 0x91 : 0x81;
  msg->db1 = track[index + 2];
  msg->db2 = track[index + 3];
  
  return (index + 4);
}

void sdsource_playfile()
{
  draw_midi_volbar();
  
  unsigned long lt = millis();
  unsigned long kt = 0;
  unsigned long ct = 0;
  unsigned long st = millis();
  
  for (;;)
  {
    unsigned long t = millis();
    unsigned char key = get_key();
    if (enc_pos_changed)
    {
      draw_midi_volbar();
      enc_pos_changed = 0;
    }    
    if (t - kt > 100)
    {
      if (key == btnSELECT)
      {
        note_stop();
        kt = t;
        return;
      }
      else if (key == btnUP)
      {
        //incvol(&lcd);
        volindex++;
        draw_midi_volbar();
        kt = t;
      }
      else if (key == btnDOWN)
      {
        //decvol(&lcd);
        volindex--;
        draw_midi_volbar();
        kt = t;
      }
    }

    if (t - ct > 1000)
    {
      unsigned int tsec = (int) ((t - st) / 1000);
      unsigned int lsec = (int) (sd->len / 1000);
      
      oled.setCursor(0, OLED_LINE3);
      oled.clearToEOL();
      oled.setCursor(40, OLED_LINE3);
      oled.print(tsec / 60);
      oled.print(":");
      //lcd_printat(2, 0, 13, tsec / 60);
      //lcd_print(':');
      unsigned int tmp = tsec % 60;
      if (tmp < 10)
      {
        oled.print("0");
        //lcd_print('0');
      }
      oled.print(tmp);
      oled.print("/");
      oled.print(lsec / 60);
      oled.print(":");
      //lcd_print(tmp);
      //lcd_print('/');
      //lcd_print(lsec / 60);
      //lcd_print(':');
      tmp = lsec % 60;
      if (tmp < 10)
      {
        oled.print("0");
        //lcd_print('0');
      }
      oled.print(tmp);
      //lcd_print(tmp);
      ct = t;
    }
    if (t - lt > sd->next->time)
    {
      sd->time += (t - lt);
      sd->p_msg->cmd = sd->next->cmd;
      sd->p_msg->db1 = sd->next->db1;
      sd->p_msg->db2 = sd->next->db2;
      parsemsg(sd->p_msg);
      sd->buf_index = read_msg(sd->buf, sd->buf_index, sd->next);
      if (sd->read_count == 0)
      {
        return;
      }
      lt = t;
    }
  }
}

