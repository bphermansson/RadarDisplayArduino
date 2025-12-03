#pragma once
#include <string>
#include "AppSettings.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h> // for String

class Weather {
public:
    int temperature;   // degrees Celsius
    String forecast;   // e.g. "Sunny", "Rain"

    Weather();
    void print() const;
};
