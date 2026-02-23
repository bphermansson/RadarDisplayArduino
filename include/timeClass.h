#pragma once

class Time {
public:
  char my_time[20];
  Time();
  void initTime();
  void localTime();
  String getTimeString();
  String getHourString();
  String getDateString(); 
  String getWeekdayString(); 
};
