#include "Weather.h"
#include <Arduino.h>

Weather::Weather() {
    temperature = 0;
    forecast = "";
}

void Weather::print() const {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Forecast: ");
    Serial.println(forecast);
}
