#include <Arduino.h>
#include <WiFi.h>
#include "Weather.h"
#include "WeatherFetcher.h"
#include "epaper.h"

Weather currentWeather;   
unsigned long startTime;
const unsigned long waitMs = 10 * 60 * 1000UL; // 3 minuter
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
    InitialiseDisplay();
    fetchWeather(currentWeather);
    startTime = millis();
    clearScreen();
    currentWeather.printserial();
    currentWeather.printepaper();
}

void loop() {
  if (millis() - startTime >= waitMs) {
    Serial.println("3 minuter har passerat!");
    startTime = millis(); 
    fetchWeather(currentWeather);
    currentWeather.printserial();
    currentWeather.printepaper();
  }
}

