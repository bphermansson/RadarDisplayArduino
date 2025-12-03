#include <Arduino.h>
#include <WiFi.h>
#include "Weather.h"
#include "WeatherFetcher.h"
#include "AppSettings.h"

Weather currentWeather;   // global instance

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

    // Call the external function which fills the Weather object
    fetchWeather(currentWeather);

    // Use the filled object
    currentWeather.print();
}

void loop() {
    delay(10000);
    Serial.println("Hello from ESP32-S3!");

   // fetchWeather(currentWeather);
    //currentWeather.print();
  }

