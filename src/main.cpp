#include <Arduino.h>
#include <WiFi.h>
#include "weatherClass.h"
#include "WeatherFetcher.h"
#include "epaper.h"
#include "timeClass.h"

Weather currentWeather;   
Time timeObj;
unsigned long startTime;
unsigned long startTimeClock;
const unsigned long waitMs = 15 * 60 * 1000UL; 
const unsigned long waitMsClock = 1 * 60 * 1000UL; 
bool firstRunClock = true;
bool firstRun = true;
int updateCounter = 0;
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;


void setup() {
    Serial.begin(115200);
    delay(8000);
    Serial.println("Hello from ESP32-S3 setup!");
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    initialiseDisplay();
    timeObj.initTime();
}

void loop() {
  if (millis() - startTimeClock >= waitMsClock || firstRunClock) {
    firstRunClock = false;
    startTimeClock = millis(); 
    timeObj.localTime();
    String timeText = timeObj.getTimeString();
    partialRefresh(145, 8, 100, 35, 4, 32, timeText);
  }
  if (millis() - startTime >= waitMs || firstRun) {
    firstRun = false;
    startTime = millis(); 
    fetchWeather(currentWeather);
    currentWeather.printserial();

    String tempText = String(currentWeather.temperature,1) + "°C";
    partialRefresh(8, 8, 130, 35, 4, 32, tempText);

    String timeText = String(timeObj.getTimeString());
    partialRefresh(130, 8, 110, 35, 4, 32, timeText);

    String forecastText = currentWeather.getSwedishForecast();
    partialRefresh(10, 65, 200, 25, 2, 18, forecastText);

    String weekDayText = timeObj.getWeekdayString();
    partialRefresh(10, 100, 200, 25, 2, 18, weekDayText);

    String dateText = timeObj.getDateString();
    partialRefresh(110, 100, 200, 25, 2, 18, dateText);

    


  }
}

