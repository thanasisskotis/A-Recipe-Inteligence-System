// The file for interacting with the RTC module
// Encapsulation 

#include <ThreeWire.h> 
#include <RtcDS1302.h>
#include "rtc.h"


// might change later 
// IO / DAT = 6
// SCLK / CLK = 7

// CE / RST =  5
ThreeWire myWire(6, 7, 5); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

// This function to be called within setup 
// to start the RTC and initialize it with the current time
// returns true if it is running correctly, else false
// init_rtc(YY, MM, DD, HH, MM, SS)
void init_rtc(const char* date, const char* time) {
    rtc.Begin();
    
    const RtcDateTime compile_time = RtcDateTime(date, time);
    
    rtc.SetDateTime(compile_time);

    RtcDateTime current_time = rtc.GetDateTime();
    
    if (rtc.GetIsWriteProtected()) {
        rtc.SetIsWriteProtected(false);
    }

    if (!rtc.GetIsRunning()) {
        rtc.SetIsRunning(true);
    }

    RtcDateTime now = rtc.GetDateTime();
    if (now < compile_time) {
        rtc.SetDateTime(compile_time);
    }

    
}

// returns the day {1:M, 2:T, 3:W, 4:T, 5:F, 6:S, 7:S}
uint8_t Day() {
    RtcDateTime current_time = rtc.GetDateTime();
    uint8_t day = current_time.DayOfWeek();
    return day == 0 ? 7 : day;
}

// returns the month 
uint8_t Month() {
    RtcDateTime current_time = rtc.GetDateTime();
    return current_time.Month();
}

// returns an unsigned long in the format MMDDHHMMSS
// based on the current time
unsigned long datetime_seed() {
    RtcDateTime curr = rtc.GetDateTime();
    return curr.Month()*100000000 + curr.Day()*1000000 + curr.Hour()*10000 + curr.Minute()*100 + curr.Second();
}
