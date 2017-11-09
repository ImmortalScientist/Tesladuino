#ifndef __UTIL_H
#define __UTIL_H
#include "datatypes.h"

//note handlng
void note_kill(note *n);
void make_note(note *n, unsigned char pitch, unsigned char velocity);
//frequency utilities
unsigned int get_on_time(long freq);
unsigned long get_freq(unsigned char note);
//keypresses
unsigned char get_key();
//volume bar
//file utils
unsigned char chk_ext(char* fname);

#endif
