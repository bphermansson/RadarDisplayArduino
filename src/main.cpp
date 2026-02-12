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
bool wasDetecting = false;
const unsigned long RADAR_STALE_TIMEOUT_MS = 500;
const unsigned long MOTION_CONFIRM_MS = 400;
const unsigned long NO_MOTION_CONFIRM_MS = 400;
unsigned long motionCandidateSince = 0;
unsigned long noMotionSince = 0;

void setup() {
    Serial.begin(115200);
    delay(8000);
    Serial.println("Hello from setup!");
    
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

    // Initialize Serial for LD2420
    mySerial.begin(115200, SERIAL_8N1, RX_PORT, RADAR_OT1_PIN);  // HLK-LD2420 baud rate (256000), RX2=16, TX2=17
    
    // Initialize the radar sensor
    if (radar.begin(mySerial)) {
      Serial.println("Radar ready. Waiting for motion...");
    } else {
      Serial.println("Failed to initialize radar!");
      while (1) delay(1000);
    }
    
    // Configure sensor
    radar.setDistanceRange(MIN_RANGE, MAX_RANGE);
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
    
    // Treat stale radar data as no detection so stop events are emitted.
    const unsigned long now = millis();
    const bool hasFreshData = radar.isDataValid() &&
                              (now - radar.getLastUpdateTime() <= RADAR_STALE_TIMEOUT_MS);
    const bool rawDetecting = hasFreshData && radar.isDetecting();
    int currentDistance = radar.getDistance();

    if (rawDetecting) {
      noMotionSince = 0;
      if (motionCandidateSince == 0) {
        motionCandidateSince = now;
      }
    } else {
      motionCandidateSince = 0;
      if (noMotionSince == 0) {
        noMotionSince = now;
      }
    }

    bool isDetecting = wasDetecting;
    if (!wasDetecting) {
      if (rawDetecting && (now - motionCandidateSince >= MOTION_CONFIRM_MS)) {
        isDetecting = true;
      }
    } else {
      if (!rawDetecting && (now - noMotionSince >= NO_MOTION_CONFIRM_MS)) {
        isDetecting = false;
      }
    }

    if (isDetecting && !wasDetecting) {
      Serial.println("Motion detected");
      blink();
    } else if (!isDetecting && wasDetecting) {
      Serial.println("Motion stopped");
      lastDistance = -1;
    }

    // Only print when distance changes while detection is active.
    if (isDetecting && currentDistance != lastDistance) {
      Serial.print("Motion detected at ");
      Serial.print(currentDistance);
      Serial.println("cm");
      lastDistance = currentDistance;
    }

    wasDetecting = isDetecting;
}
