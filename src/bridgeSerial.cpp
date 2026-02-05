#include <bridgeSerial.h>
extern HardwareSerial mySerial;
//Function to bridge the serial communication between the module and the serial monitor
//Checks the running mode and prints formated data in HEX, check https://github.com/JoaoSandrini/LD2420-Radar/blob/main/HLK-LD2420%20Protocol%20(Translated).pdf
void bridgeSerial() {
  std::vector<byte> debugFooter = {0xFD, 0xFC, 0xFB, 0xFA};
  std::vector<byte> reportFooter = {0xF8, 0xF7, 0xF6, 0xF5};
  std::vector<byte> checkFooter;

  if (mySerial.available() > 0) {
    Serial.print(mySerial.read(), HEX);
    Serial.print(" ");

    if (mySerial.peek() == 0xF8) {
      for (int i = 0; i < 4; i++) {
        checkFooter.push_back(mySerial.read());
      }
      if (checkFooter == reportFooter) {
        for (byte b : checkFooter) {
          Serial.print(b, HEX);
          Serial.print(" ");
        }
        Serial.println();
      }
    }

    if (mySerial.peek() == 0xFD) {
      for (int i = 0; i < 4; i++) {
        checkFooter.push_back(mySerial.read());
      }
      if (checkFooter == debugFooter) {
        for (byte b : checkFooter) {
          Serial.print(b, HEX);
          Serial.print(" ");
        }
        Serial.println();
      }
    }
  }
}