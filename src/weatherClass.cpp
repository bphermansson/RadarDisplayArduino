#include <Arduino.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "weatherClass.h"
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
String Weather::getSwedishForecast() {
    String f = forecast;
    f.toLowerCase();
    
    // Klar himmel / Klart
    if (f.indexOf("clear") >= 0 || f.indexOf("fair") >= 0) {
        if (f.indexOf("night") >= 0) {
            return "Klar natt";  // ✅ fair_night / clear_night
        }
        return "Klart";  // clear_day / fair_day
    }
    
    // Soligt (dag)
    if (f.indexOf("sunny") >= 0) {
        return "Soligt";
    }
    
    // Delvis molnigt
    if (f.indexOf("partly_cloudy") >= 0 || f.indexOf("partly cloudy") >= 0 || f.indexOf("partly") >= 0) {
        if (f.indexOf("night") >= 0) {
            return "Delvis molnig natt";
        }
        return "Delvis molnigt";
    }
    
    // Mest molnigt
    if (f.indexOf("mostly_cloudy") >= 0 || f.indexOf("mostly cloudy") >= 0) {
        return "Mest molnigt";
    }

    if (f.indexOf("partlycloudy_day") >= 0) {
        return "Växlande  molnighet";
    }
    // Molnigt
    if (f.indexOf("cloudy") >= 0 || f.indexOf("overcast") >= 0) {
        return "Molnigt";
    }
    
    // Regn
    if (f.indexOf("light_rain") >= 0 || f.indexOf("light rain") >= 0) {
        return "Lätt regn";
    }
    if (f.indexOf("rain") >= 0 || f.indexOf("showers") >= 0) {
        return "Regn";
    }
    if (f.indexOf("drizzle") >= 0) {
        return "Duggregn";
    }
    
    // Snö
    if (f.indexOf("snow") >= 0) {
        return "Snö";
    }
    
    // Åska
    if (f.indexOf("thunder") >= 0) {
        return "Åska";
    }
    
    // Dimma
    if (f.indexOf("fog") >= 0 || f.indexOf("mist") >= 0) {
        return "Dimma";
    }
    
    // Fallback - visa original om okänd
    return forecast;
}