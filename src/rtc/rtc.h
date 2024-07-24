#ifndef rtc_h
#define rtc_h


#include "Arduino.h"
#include "ThreeWire.h" 
#include "RtcDS1302.h"


void init_rtc(const char* compile_date, const char* compile_time);

uint8_t day();

uint8_t month();

unsigned long datetime_seed();

#endif
