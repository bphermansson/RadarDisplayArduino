#include "WeatherFetcher.h"
#include <Arduino.h>
#include <WiFi.h>
#include "AppSettings.h"
#include "weather_codes.h"

void fetchWeather(Weather &out) {

    WiFiClient client;
    HTTPClient http;

    http.begin(smhi_temp_url);
    int httpCode = http.GET();

    if (httpCode == 200) {
    String payload = http.getString();

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (!err) {
        // Temperature is normally entry[0].parameters[x].values[0]
        JsonArray parameters = doc["timeSeries"][0]["parameters"];
        float temp = NAN;

        for (JsonObject p : parameters) {
        const char* name = p["name"];
        if (strcmp(name, "t") == 0) {     // "t" = temperature
            temp = p["values"][0];
            break;
        }
        }
        //Serial.println("Tick!");

        if (!isnan(temp)) {
            //Serial.printf("Current temperature: %.1f°C\n", temp);
            out.temperature = temp;            // set temperature
        }
    }
    } else {
            Serial.printf("HTTP error: %d\n", httpCode);
    }
    http.end();
    
    http.begin(smhi_forecast_url);
    httpCode = http.GET();

    if (httpCode == 200) {
    String payload = http.getString();

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (!err) {
        int weatherCode = doc["value"][0]["value"].as<int>(); // 158
        //Serial.printf("Current weather: %1d\n", weatherCode);
        //Serial.println(decodeWeather(weatherCode));
        out.forecast = decodeWeather(weatherCode);    // set forecast text
    }
    } else {
            Serial.printf("HTTP error: %d\n", httpCode);
    }
    http.end();
    // Optionally log
    //Serial.println("fetchWeather(): filled Weather instance");
}
