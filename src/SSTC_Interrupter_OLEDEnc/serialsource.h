#ifndef __SERIALSOURCE_H
#define __SERIALSOURCE_H

//#define BAUDRATE 31250    // MIDI baud rate
#define BAUDRATE 38400    // testing  baud rate
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init();

void serialsource_init();
void serialsource_run();
#endif
