#include "WeatherFetcher.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* YR_USER_AGENT = "MyWeatherApp/1.0 github.com/bphermansson/RadarDisplayArduino";

void fetchWeather(Weather &out) {
    WiFiClient client;
    HTTPClient http;

    char url[150];
    snprintf(url, sizeof(url), 
    "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=%.2f&lon=%.2f",
    LATITUDE, LONGITUDE);

    http.begin(url);
    http.addHeader("User-Agent", YR_USER_AGENT);
    
    int httpCode = http.GET();
    
    if (httpCode == 200) {
        String payload = http.getString();
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, payload);
        
        if (!err) {
            // Hämta nuvarande temperatur
            float temp = doc["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"];
            out.temperature = temp;
            
            // Hämta väder-symbol för prognos
            const char* symbol = doc["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"];
            out.forecast = String(symbol);  // t.ex. "cloudy", "rain", "clearsky"
            
            Serial.printf("Temp: %.1f°C, Forecast: %s\n", temp, symbol);
        } else {
            Serial.printf("JSON parse error: %s\n", err.c_str());
        }
    } else {
        Serial.printf("HTTP error: %d\n", httpCode);
    }
    
    http.end();
}