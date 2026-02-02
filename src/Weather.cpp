#include <Arduino.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "Weather.h"
#include "epaper.h"

Weather::Weather() {
    temperature = 0;
    forecast = "";
}

void Weather::printserial() const {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Forecast: ");
    Serial.println(forecast);
    Serial.print(millis());
}
