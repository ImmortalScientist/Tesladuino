#include "OLED.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "data.h"
#include "constants.h"

#define I2C_ADDRESS   0x3C
#define OPTMIZE_I2C   1

extern SSD1306AsciiWire oled;

extern volatile int ffreq;
extern volatile char volindex;
extern volatile char mode;

void oled_init(void)
{
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  TWBR = 0;   // set the i2c to 1MHz
  oled.setFont(Arial14);
  oled.clear();
}

void draw_fixed_screen(void)
{ 
  if (volindex > 60)
  {
    volindex = 60; 
  }
  
  if (mode == ADJ_MODE_FREQ)
  {    
    oled.setCursor(0, OLED_LINE3);
    oled.clearToEOL();
    oled.print("[");
    oled.setCursor(4, OLED_LINE3);
    oled.print("Freq: ");
    oled.print(ffreq);
    oled.print("Hz");
    oled.setCursor(125, OLED_LINE3);
    oled.print("]");
    oled.setCursor(0, OLED_LINE4);
    oled.clearToEOL();
    oled.setCursor(4, OLED_LINE4);
    //sprintf(volbar, "");
    for (int i = 0; i < volindex; i++)
    {
      //strcat(volbar, "|");
      oled.print("|");
    }
    //oled.write(volbar);
  }

  else if (mode == ADJ_MODE_VOL)
  {  
    oled.setCursor(0, OLED_LINE3);
    oled.clearToEOL();
    oled.setCursor(4, OLED_LINE3);
    oled.print("Freq: ");
    oled.print(ffreq);
    oled.print("Hz");
    oled.setCursor(0, OLED_LINE4);
    oled.clearToEOL();
    oled.print("[");
    oled.setCursor(4, OLED_LINE4);
    //sprintf(volbar, "");
    for (int i = 0; i < volindex; i++)
    {
      //strcat(volbar, "|");
      oled.print("|");
    }
    //oled.write(volbar);
    oled.setCursor(125, OLED_LINE4);
    oled.print("]");
  }  
}

void draw_midi_volbar(void)
{
    if (volindex > 60)
    {
      volindex = 60; 
    }
    oled.setCursor(0, OLED_LINE4);
    oled.clearToEOL();
    oled.print("[");
    oled.setCursor(4, OLED_LINE4);
    //sprintf(volbar, "");
    for (int i = 0; i < volindex; i++)
    {
      //strcat(volbar, "|");
      oled.print("|");
    }
    //oled.write(volbar);
    oled.setCursor(125, OLED_LINE4);
    oled.print("]");  
}

