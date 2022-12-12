#include "display_mycraft.h"

uint8_t pos_x = 0,
        step_x = 4;
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
  last = millis();
  u8g2.setFont(u8g2_font_samim_16_t_all);
  u8g2.setCursor(pos_x, 30);
  u8g2.setDrawColor(0);
  u8g2.drawBox(pos_x - step_x, 15, pos_x + u8g2.getUTF8Width("My craft"), 30);
  u8g2.setDrawColor(1);
  pos_x += step_x;
  pos_x = pos_x > 127 ? 0 - u8g2.getUTF8Width("My craft") : pos_x;
  u8g2.printf("My craft");
  return true;
}