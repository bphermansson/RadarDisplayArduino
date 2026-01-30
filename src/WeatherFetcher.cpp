    http.begin(smhi_temp_url);

    http.begin(smhi_forecast_url);

#include "WeatherFetcher.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Yr.no kräver User-Agent header
const char* YR_USER_AGENT = "MyWeatherApp/1.0 github.com/yourusername/yourapp";

void fetchWeather(Weather &out) {
    WiFiClient client;
    HTTPClient http;
    
    // Yr.no API endpoint (ersätt lat/lon med din position)
    // Uddevalla ungefär: lat=58.35, lon=11.93
    String url = "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=58.35&lon=11.93";
    
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