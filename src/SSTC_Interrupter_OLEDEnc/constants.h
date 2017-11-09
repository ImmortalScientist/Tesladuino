/*
 * constants.h
 * Contains defined constants, user-modifiable macros, and structures
 * Read the manual to see what to modify
 */
 
#ifndef __CONSTANTS_H

#include "system.h"

//#define DEBUG

/****Coil-dependent configuration variables: change these to match your coil****/

//multiply the pulsewidths by this constant, good for quick 'n dirty scaling
#define LOOKUP_TABLE_SCALE 3

//minimum on-time, in microseconds
#define MIN_ON_TIME 10

//on-times in microseconds, don't forget to multiply by LOOKUP_TABLE_SCALE!
#define ON_TIME_ARRAY_LENGTH 26
static byte on_times[] = {33, 33, 33, 33, 27, 23, 20, 20, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 8, 8, 8, 8, 8, 8};

/****End coil config variables****/

/****Instrument-dependent configuration variables: change to match your MIDI device****/

//sustain time, in microseconds
#define SUSTAIN_TIME 750000
#define SUSTAIN_TIME_OVER_256 2930

//lets us change the way velocity mapping works (linear, LUT, something else...)
#define map_velocity(v) (127 + v / 2)

//map the sustain time as well
#define map_sustain(t) ((long) (t) / SUSTAIN_TIME_OVER_256)
/****End instrument config variables****/

/****System stuff****/
#define FILENAME_SIZE       256       // <-- Don't change this, makes playback unstable.

//#define FILE_BUFFER_SIZE  512       // <-- Doesn't fit anymore.
#define FILE_BUFFER_SIZE  384       // <-- Also doesn't fit anymore.
//#define FILE_BUFFER_SIZE  256       // <-- Even better, but still hangs on long files.
//#define FILE_BUFFER_SIZE  128       // <-- Better, but still hangs on long files.
//#define FILE_BUFFER_SIZE  64        // <-- Don't change to this, makes playback unstable.

#define MMC_CS        10
#define BTN_RED		  10
#define BTN_ENC		  17
#define ENCODER_A     A1
#define ENCODER_B     A2

#define MAX_FREQ      1000
#define MIN_FREQ      1

#define ADJ_MODE_NONE   0
#define ADJ_MODE_FREQ   1
#define ADJ_MODE_VOL    2
#define ADJ_MODE_FILE   3
#define ADJ_MODE_MENU   4

//don't mess with these!
#define BITS_8 256
#define BITS_16 65536
#define TIMER_2_PERIOD_MAX 16000

static unsigned int base[] = {220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415};
static unsigned char pow2[] = {1, 2, 4, 8, 16, 32};
static char chex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
static unsigned char block[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static unsigned char arrow[8] = {~0x0f, ~0x07, ~0x03, ~0x01, ~0x03, ~0x07, ~0x0f, 0x00};

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define btnRETURN 6

#define MENU_SDCARD 0
#define MENU_FIXED  1
#define MENU_LIVE   2

#define OLED_LINE1              0
#define OLED_LINE2              2
#define OLED_LINE3              4
#define OLED_LINE4              6

#define __CONSTANTS_H
#endif
