// The file for interacting with the RTC module
#include <ThreeWire.h> 
#include <RtcDS1302.h>
#include "rtc.h" 

// might change later 
uint8_t CLK_PIN = 10;
uint8_t DAT_PIN = 9;
uint8_t RST_PIN = 8;

ThreeWire myWire(DAT_PIN, CLK_PIN, RST_PIN); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

// This function to be called within setup 
// to start the RTC and initialize it with the current time
// returns true if it is running correctly, else false
// init_rtc(YY, MM, DD, HH, MM, SS)
void init_rtc(const char* date, const char* time) {
    rtc.Begin();

    // Check if the RTC is running
    if (!rtc.GetIsRunning()) {
        Serial.println("RTC is not running, starting now.");
        rtc.SetIsRunning(true);
    }
    
    // Run only once to set the RTCs time to the current time or if the time diplayed is incorrect
    // const RtcDateTime compile_time = RtcDateTime(date, time);
    // rtc.SetDateTime(compile_time);
    
}

char* Datestamp() {
    char* buf = malloc(16); // must be freed by the calling function!
    RtcDateTime current_time = rtc.GetDateTime();
    
    char* DayOfTheWeek[7] = {"KYRIAKH", "DEYTERA", "TRITH", "TETARTH", "PEMPTH", "PARASKEBH", "SABBATO"};
    char* today = DayOfTheWeek[current_time.DayOfWeek()];

    sprintf(buf, "%s %d/%d", today, current_time.Day(), current_time.Month());
    return buf;
}

char* Timestamp() {
    char* buf = malloc(16); // must be freed by the calling function!
    RtcDateTime current_time = rtc.GetDateTime();

    sprintf(buf, "%d %d:%d", current_time.Year(), current_time.Hour(), current_time.Minute());
    return buf;
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
