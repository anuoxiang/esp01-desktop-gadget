#include "display_helloworld.h"

uint8_t pos_x_h = 32;
bool hello_main()
{
  u8g2.setFont(u8g2_font_6x13_tf);
  u8g2.setCursor(pos_x_h, 16);
  u8g2.setDrawColor(0);
  u8g2.drawBox(pos_x_h - 1, 0, pos_x_h + u8g2.getUTF8Width("Hello"), 16);
  u8g2.setDrawColor(1);
  pos_x_h++;
  pos_x_h = pos_x_h > 127 ? 0 - u8g2.getUTF8Width("Hello") : pos_x_h;
  u8g2.printf("Hello");
  return true;
}