#include <Arduino.h>
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
void Weather::printepaper() const {
    uint8_t distance = 20;
    uint8_t startX = 30;
    uint8_t startY = 30;
    clearScreen();
    String text = "Ute: " + String(temperature) + " C" ;
    u8g2Fonts.setFont(u8g2_font_helvB24_tf);
    drawString(startX, startY + 0 * distance, text);

    startX = 5;
    startY = 70;
    text = "Väder: " + String(forecast) + "." ;
    u8g2Fonts.setFont(u8g2_font_helvB14_tf);
    drawString(startX, startY + 0 * distance, text);
    display.display(false); // Full screen update mode 
    display.hibernate();
}