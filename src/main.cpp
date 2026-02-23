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
bool detecting = false;
int updateState = 0;
int lastDistance = -1;
const unsigned long RADAR_STALE_TIMEOUT_MS = 500;
const unsigned long MOTION_IDLE_TIMEOUT_MS = 10000;
unsigned long lastDistanceChangeAt = 0;
int updateCounter = 0;
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
extern HardwareSerial mySerial(2); 
LD2420 radar;  // Use default constructor

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
    mySerial.begin(115200, SERIAL_8N1, RADAR_UART_RX_PIN, RADAR_UART_TX_PIN);  // LD2420 TX -> ESP RX, LD2420 RX <- ESP TX
    
    // Initialize the radar sensor
    if (radar.begin(mySerial)) {
      Serial.println("Radar ready. Waiting for motion...");
    } else {
      Serial.println("Failed to initialize radar!");
      while (1) delay(1000);
    }
    
    // Configure sensor
    radar.setDistanceRange(MIN_RANGE, MAX_RANGE);
    radar.setUpdateInterval(10); // 20Hz update rate

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
  bool inFullUpdate = false;

  if (millis() - startTime >= waitMs || firstRun || updateState > 0  ) {
    firstRun = false;
    startTime = millis(); 
    
    fetchWeather(currentWeather);
    //currentWeather.printserial();
    switch (updateState)
    {
    case 0: {
      Serial.println("Starting full update... this may take a moment." );
      inFullUpdate = true;
      String tempText = String(currentWeather.temperature,1) + "°C";
      partialRefresh(8, 8, 130, 35, 4, 32, tempText);
      updateState++;
      Serial.println(updateState);
      break;
    }
    case 1: {
      Serial.println("Step 1 done, updating temp..." );
      String tempText = String(currentWeather.temperature,1) + "°C";
      partialRefresh(8, 8, 130, 35, 4, 32, tempText);
      updateState++;
      break;
    }
    case 2: {
      Serial.println("Step 2 done, updating time..." );
      String timeText = String(timeObj.getTimeString());
      partialRefresh(130, 8, 110, 35, 4, 32, timeText);
      updateState++;
      break;
    }
    case 3: {
      Serial.println("Step 3 done, updating forecast..." );
      String forecastText = currentWeather.getSwedishForecast();
      partialRefresh(10, 65, 200, 25, 2, 18, forecastText);
      updateState++;
      break;  
    }
    case 4: {
      Serial.println("Step 4 done, updating weekday..." );
      String weekDayText = timeObj.getWeekdayString();
      partialRefresh(10, 100, 200, 25, 2, 18, weekDayText);
      updateState++;
      break;
    }
    case 5: {
      Serial.println("Step 5 done, updating date..." );
      String dateText = timeObj.getDateString();
      partialRefresh(110, 100, 200, 25, 2, 18, dateText);
      updateState++;
      break;  
    }
    case 6: {
      Serial.println("Full update done." );
      inFullUpdate = false;
      updateState = 0;
      break;  
    }
    default:
      break;
    }
  } 

  const unsigned long now = millis();
  
  // Radar
  radar.update();  // Update radar readings
  const bool hasFreshData = radar.isDataValid() &&
                            (now - radar.getLastUpdateTime() <= RADAR_STALE_TIMEOUT_MS);
  const bool isDetectingNow = hasFreshData && radar.isDetecting();

  if (isDetectingNow) {
    if (isDetectingNow && radar.getDistance() != lastDistance) {  
      if (!detecting) {
        Serial.println("Motion detected");
        detecting = true;
        if(timeObj.getHourString().toInt() >= DND_START || timeObj.getHourString().toInt() < DND_END){
          Serial.println("DND active, not blinking");
        }
        else {
          blink();
        }
      }
      String timestamp = timeObj.getDateString() + " " + timeObj.getTimeString();
      Serial.print("[");
      Serial.print(timestamp);
      Serial.print("] Motion detected at ");
      Serial.print(radar.getDistance());
      Serial.println("cm");
      lastDistance = radar.getDistance();
      lastDistanceChangeAt = now;
    }   

    if (detecting && lastDistanceChangeAt > 0 &&
        (now - lastDistanceChangeAt >= MOTION_IDLE_TIMEOUT_MS)) {
      Serial.println("Motion stopped");
      detecting = false;
      lastDistance = -1;
    }
  }
  else if (detecting) {
    Serial.println("Motion stopped");
    detecting = false;
    lastDistance = -1;
    lastDistanceChangeAt = 0;
  }
}
