#include "display_mycraft.h"

int16_t pos_x = 0;
uint8_t step_x = 4;
unsigned long last = 0;

bool mycraft_main(int16_t offsetX)
{
  Serial.printf("craft %d", offsetX);
  u8g2.setFont(u8g2_font_samim_16_t_all);
  u8g2.setCursor(20 + offsetX, 30);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 15, SCREEN_WIDTH, 63);
  u8g2.setDrawColor(1);
  pos_x += step_x;
  pos_x = pos_x > 127 ? 0 - u8g2.getUTF8Width("My craft") : pos_x;
  u8g2.printf("My craft");
  return true;
}