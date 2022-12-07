#include "display_helloworld.h"

uint8_t pos_x_h = 32;
void hello_main()
{
  u8g2.setFont(u8g2_font_6x13_tf);
  u8g2.setCursor(pos_x_h, 16);
  pos_x_h++;
  pos_x_h = pos_x_h > 127 ? 0 : pos_x_h;
  u8g2.printf("Hello");
}