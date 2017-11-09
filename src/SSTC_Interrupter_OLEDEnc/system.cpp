/*
 * System dependent functions
 * These abstract away assorted registers
 * and ATMega-specific functions
 * Edit these first when attempting to port the code
 */
 
#include "system.h"
#include "constants.h"

void setupTimers() {
  // Set up 16-bit Timer1
  TCCR1A = 0x00;          // CTC mode, hardware pins disconnected
  TCCR1B = (1 << WGM12);  // CTC mode, don't set prescaler yet
  TIMSK1 = 0x00;
  
  // Set up 16-bit Timer2
  TCCR2A = (1 << WGM21);   // CTC mode, hardware pins disconnected, don't set prescaler yet
  TCCR2B = 0x00;
  TIMSK2 = 0x00;
}

void setupPins()
{
  // Setup optical transmitter output pin
  DDRD |= (1 << 2);
  
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DN, INPUT);
  pinMode(BTN_OK, INPUT);
  pinMode(BTN_RTN, INPUT);

  pinMode(LED_RED1, OUTPUT);
  pinMode(LED_RED2, OUTPUT);
  pinMode(LED_GRN1, OUTPUT);

  // power LED ON
  digitalWrite(LED_RED2, HIGH);
}

