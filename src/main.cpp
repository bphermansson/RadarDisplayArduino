#include <Arduino.h>
#include <WiFi.h>
#include "weatherClass.h"
#include "WeatherFetcher.h"
#include "epaper.h"
#include "timeClass.h"
#include <definitions.h>
#include <HardwareSerial.h>
#include <blinkStrip.h>
#include "LD2420.h"

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
extern HardwareSerial mySerial(2); 
LD2420 radar;  // Use default constructor

// Track last distance to detect changes
int lastDistance = -1;
int detectionTime = 0;
bool wasDetecting = false;
bool newDetection = false;

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

    // Initialize SSerial for LD2420
    mySerial.begin(115200, SERIAL_8N1, RX_PORT, RADAR_OT1_PIN);  // HLK-LD2420 baud rate (256000), RX2=16, TX2=17
    
    // Initialize the radar sensor
    if (radar.begin(mySerial)) {
      Serial.println("Radar ready. Waiting for motion...");
    } else {
      Serial.println("Failed to initialize radar!");
      while (1) delay(1000);
    }
    
    // Configure sensor
    radar.setDistanceRange(100, 600);
    radar.setUpdateInterval(50); // 20Hz update rate

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
    radar.update();  // Update radar readings
    
    // Check if motion is detected
    bool isDetecting = radar.isDetecting();
    int currentDistance = radar.getDistance();
    
    if (isDetecting) {
      if (newDetection == false) {
        Serial.println("Motion detected");
        newDetection = true;
      } 
      // Only print when distance changes
      if (currentDistance != lastDistance) {
        Serial.print("Motion detected at ");
        Serial.print(currentDistance);
        Serial.println("cm");
        if (detectionTime - 10000 < 0) {
          detectionTime=millis();
          blink();
        }
        
        
        lastDistance = currentDistance;
        wasDetecting = true;
      }
     } //else {
    //   // Motion stopped - reset for next detection
    //   if (wasDetecting) {
    //     Serial.print("Motion stopped ");
    //     lastDistance = -1;
    //     wasDetecting = false;
    //     newDetection = false;
    //   }
    // }


}