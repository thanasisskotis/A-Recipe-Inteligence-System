#ifndef rtc_h
#define rtc_h


#include "Arduino.h"
#include "ThreeWire.h" 
#include "RtcDS1302.h"


void init_rtc(const char* compile_date, const char* compile_time);

char* Datestamp();

char* Timestamp();

uint8_t Day();

uint8_t Month();

unsigned long datetime_seed();

#endif
