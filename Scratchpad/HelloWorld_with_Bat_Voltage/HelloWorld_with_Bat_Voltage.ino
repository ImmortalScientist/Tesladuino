
#include <Arduino.h>
#include <avr/dtostrf.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define VBATPIN A7

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  


void setup(void) {
  u8g2.begin();
}

void loop(void) {
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 6.6;
  measuredvbat /= 1024;

  char batStr[5];
  dtostrf(measuredvbat, 4, 2, batStr);    
  
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory
  u8g2.drawStr(0,25,"Battery Voltage:        V");
  u8g2.drawStr(90,25,batStr);
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  
}

