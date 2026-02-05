#include <Arduino.h>
#include <HardwareSerial.h>
#include <blinkStrip.h>
CRGB leds_s[NUM_LEDS_S];

void blink(){
  Serial.println("\r\nBlink!");
  FastLED.setBrightness(BRIGHTNESS_S); 
  
  for(int x=0; x<=3; x++)
  {
    for(int l=0;l<=NUM_LEDS_S;l++) {
      fill_solid(leds_s, l, CRGB::Red);
      FastLED.show(); 
      delay(3);
    }
        
    for(int l=NUM_LEDS_S-1; l>=0; l--) {
      leds_s[l] = CRGB::Blue;  
      FastLED.show(); 
      delay(3);
    }
    fill_solid(leds_s, NUM_LEDS_S, CRGB::Black);
    FastLED.show(); 
  }
}