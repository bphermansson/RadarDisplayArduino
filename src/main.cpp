#include <Arduino.h>
#include <WiFi.h>
#include "Weather.h"
#include "WeatherFetcher.h"
#include "epaper.h"

Weather currentWeather;   
unsigned long startTime;
const unsigned long waitMsClock = 1 * 60 * 1000UL; 
int updateCounter = 0;
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

void setup() {
    Serial.begin(115200);
    delay(4000);
    Serial.println("Hello from ESP32-S3 setup!");
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    initialiseDisplay();
    fetchWeather(currentWeather);
    startTime = millis();
    currentWeather.printserial();
    uint8_t x = 30;
    uint8_t y = 30;
    uint8_t w = 200;
    uint8_t h = 30;
    String text = "Ute: " + String(currentWeather.temperature) + " C" ;
    partialRefresh(x, y, w, h, 3, 24, text);
}

void loop() {
  if (millis() - startTime >= waitMsClock) {
    Serial.println("1 minut har passerat!");
    startTime = millis(); 
    fetchWeather(currentWeather);
    currentWeather.printserial();

uint8_t x = 30;
    uint8_t y = 30;
    uint8_t w = 200;
    uint8_t h = 30;
    String text = "Ute: " + String(currentWeather.temperature) + " C" ;
    partialRefresh(x, y, w, h, 3, 24, text);

  }
}

