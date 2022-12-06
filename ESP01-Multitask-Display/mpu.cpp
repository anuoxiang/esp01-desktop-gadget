#include "mpu.h"

int16_t ax, ay, az, gx, gy, gz;
uint16_t dt = 100;
MPU6050 accelgyro;
bool b_mpu_setup = false;

#define MAX_DELAY 5000
#define PERIOD 100

void mpu_setup()
{
  b_mpu_setup = true;
  accelgyro.initialize();
}

ProcessInfo mpu_main(void)
{
  if (!b_mpu_setup)
    mpu_setup();
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  if (gx > 20000)
  { // 向左滚动，屏幕编号增大一位
    Serial.println("Left");
    if (current_screen < SCREEN_NUM)
    {
      turn_to_screen = current_screen + 1;
      offsetX = 128;
      Serial.println("gogo.");
    }
    dt = MAX_DELAY;
  }
  else if (gx < -20000)
  { // 向右滚动，所以屏幕编号减小一位
    Serial.println("Right");
    if (current_screen > 1)
    {
      turn_to_screen = current_screen - 1;
      offsetX = 0;
      Serial.println("gogo..");
    }
    dt = MAX_DELAY;
  }
  else if (gy > 20000)
  {
    Serial.println("Down");
    dt = MAX_DELAY;
  }
  else if (gy < -20000)
  {
    Serial.println("Up");
    dt = MAX_DELAY;
  }
  else
  {
    dt = PERIOD;
  }

  ProcessInfo result;
  result.period = dt;
  result.status = RUNNING;

  return result;
}