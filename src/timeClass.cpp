#include <Arduino.h>
#include <time.h>
#include "timeClass.h"
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = TIMEZONE*3600;        // GMT+1 (Sverige vintertid)
const int daylightOffset_sec = 3600;    // +1 timme för sommartid (CEST)

Time::Time() {
  // Constructor
  memset(my_time, 0, sizeof(my_time));
}

void Time::initTime() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(2000);
}

void Time::localTime() {
  struct tm time;
   
  if(!getLocalTime(&time)){
    Serial.println("Could not obtain time info");
    return;
  }
  snprintf(my_time, 17, "%d-%02d-%02d %02d:%02d\n", (time.tm_year+1900), time.tm_mon+1,time.tm_mday, time.tm_hour, time.tm_min);
  Serial.println(my_time);
}
String Time::getTimeString() {
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    return "??:??";
  }
  
  char buffer[6];
  snprintf(buffer, 6, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  return String(buffer);
}

String Time::getDateString() {
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    return "----_--_--";
  }
  
  char buffer[11];
  snprintf(buffer, 11, "%d-%02d-%02d", 
           timeinfo.tm_year + 1900,
           timeinfo.tm_mon + 1,
           timeinfo.tm_mday);
  return String(buffer);
}
String Time::getWeekdayString() {
    struct tm timeinfo;
    
    if(!getLocalTime(&timeinfo)){
        return "???";
    }
    
    // Svenska veckodagar
    const char* weekDays[] = {
        "Söndag",    // 0
        "Måndag",    // 1
        "Tisdag",    // 2
        "Onsdag",    // 3
        "Torsdag",   // 4
        "Fredag",    // 5
        "Lördag"     // 6
    };
    
    return String(weekDays[timeinfo.tm_wday]);
}