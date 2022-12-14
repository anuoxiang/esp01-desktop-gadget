#include "display_helloworld.h"

int16_t pos_x_h = 0;
bool hello_main(int16_t offsetX)
{
  Serial.printf("hello %d", offsetX);
  u8g2.setFont(u8g2_font_6x13_tf);
  u8g2.setCursor(10 + offsetX, 14);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, SCREEN_WIDTH, 16);
  u8g2.setDrawColor(1);
  pos_x_h++;

  if (pos_x_h > 127)
    pos_x_h = 0 - u8g2.getUTF8Width("Hello");

  u8g2.printf("Hello");
  return true;
}