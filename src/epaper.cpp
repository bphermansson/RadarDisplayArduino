#include "epaper.h"
GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // GDEY0213Z98 122x250, SSD1680
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

void InitialiseDisplay() {
  display.init(115200, true, 2, false);
  SPI.end();
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);
  display.setRotation(1);                     // Use 1 or 3 for landscape modes
  u8g2Fonts.begin(display);                   // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);              // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);    // Explore u8g2 fonts from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
}
void clearScreen(){
    display.fillScreen(GxEPD_WHITE);
    display.nextPage();
}
void drawString(int x, int y, String text)
{
    int16_t x1, y1; // the bounds of x,y and w and h of the variable 'text' in pixels.
    uint16_t w, h;
    display.setTextWrap(false);
    display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
    u8g2Fonts.setCursor(x, y + h);
    u8g2Fonts.print(text);
    display.display(false); // Full screen update mode
    // Important use bring the display in one of the two modes to
    // avoid a damage of the display that cannot get repaired.
    //display.powerOff();
    display.hibernate();
}