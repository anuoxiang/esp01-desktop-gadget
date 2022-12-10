#include "display_mycraft.h"

uint8_t pos_x = 0;
unsigned long last = 0;

bool mycraft_main()
{
  if (last == 0)
  {
    last = millis();
  }
  else if (millis() - last < 100)
  {
    return false;
  }
  u8g2.setFont(u8g2_font_samim_16_t_all);
  u8g2.setCursor(pos_x, 30);
  pos_x += 10;
  pos_x = pos_x > 127 ? 0 : pos_x;
  u8g2.printf("My craft");
  return true;
}