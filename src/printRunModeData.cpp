#include <printRunModeData.h>
#include <blinkStrip.h>

extern HardwareSerial mySerial;

void printRunModeData() {
  static int currentRange = 999;
  static bool hasTriggered = false;
  static unsigned long lastActiveTime = 0; // Håller koll på NÄR vi senast såg någon
  const unsigned long timeoutPeriod = TIMEOUT_PERIOD; 

  if (mySerial.available()) {
    String line = mySerial.readStringUntil('\n');
    line.trim();

    // 1. Uppdatera avståndet
    if (line.startsWith("Range")) {
      currentRange = line.substring(6).toInt();
    }

    // 2. Hantera "ON" (Närvaro)
    if (line.indexOf("ON") >= 0) {
      if (currentRange >= MIN_RANGE) {
          Serial.print("--- Motion! --- Range: ");
          Serial.println(currentRange);
      }

      if (currentRange >= MIN_RANGE && currentRange <= MAX_RANGE) { 
        lastActiveTime = millis(); // Uppdatera timern så fort vi ser rörelse i zonen
        
        if (!hasTriggered) {
          Serial.print("--- TRIGGER: Target entered! --- Range: ");
          Serial.println(currentRange);
          blink();
          hasTriggered = true;
        }
      }
    } 
    
    // 3. Hantera "OFF" (Normal avslutning)
    else if (line.indexOf("OFF") >= 0) {
      if (hasTriggered) {
        Serial.println("--- RESET: Target left (OFF received) ---");
        // ColorLightStrip.setOnOff(false); 
        hasTriggered = false;
      }
    }
  }

  // --- FAILSAFE / TIMEOUT LOGIK ---
  // Om vi är i "Trigger-läge" men inte sett en ON-signal inom zonen på 30 sekunder
  if (hasTriggered && (millis() - lastActiveTime > timeoutPeriod)) {
    Serial.println("--- TIMEOUT: No presence detected for 30s. Forcing reset. ---");
    // ColorLightStrip.setOnOff(false); 
    hasTriggered = false; 
  }
}