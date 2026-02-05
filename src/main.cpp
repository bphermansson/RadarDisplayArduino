#include <Arduino.h>
#include <WiFi.h>
#include "weatherClass.h"
#include "WeatherFetcher.h"
#include "epaper.h"
#include "timeClass.h"
#include <constructPackage.h>
#include <definitions.h>
#include <printRunModeData.h>
#include <bridgeSerial.h>
#include <HardwareSerial.h>
#include <blinkStrip.h>

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
uint32_t lastReading = 0;
bool radarConnected = false;
extern HardwareSerial mySerial(2);  // Use UART2 (TX2/RX2)
extern byte sysModeParam = 0x00;

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
    FastLED.addLeds<LED_TYPE_S, LED_PIN_S, COLOR_ORDER_S>(leds_s, NUM_LEDS_S).setCorrection(TypicalLEDStrip);

    mySerial.begin(115200, SERIAL_8N1, RX_PORT, RADAR_OT1_PIN);  
    Serial.println("HLK-LD2420 Initialized");
    pinMode(RADAR_OT1_PIN, INPUT);   
    pinMode(RADAR_OT2_PIN, INPUT); 
    sendCommand(sysMode(RUN_MODE)); 
    getCommandResponse();
    blink();
}

void loop() {
  if (millis() - startTimeClock >= waitMsClock || firstRunClock) {
    firstRunClock = false;
    startTimeClock = millis(); 
    timeObj.localTime();
    String timeText = timeObj.getTimeString();
    partialRefresh(130, 8, 110, 35, 4, 32, timeText);
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
    // Radar
    if (sysModeParam == RUN_MODE) {
      printRunModeData();
    } else {
      bridgeSerial();
    }
}