#include "mpu.h"

int16_t ax, ay, az, gx, gy, gz;
uint16_t dt = 100;
MPU6050 accelgyro;
bool b_mpu_setup = false;

void mpu_setup()
{
  b_mpu_setup = true;
  accelgyro.initialize();
}

int8_t mpu_main(void)
{
  if (!b_mpu_setup)
    mpu_setup();
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  if (gx > 20000)
  {
    Serial.println("Left");
    dt = 2000;
  }
  else if (gx < -20000)
  {
    Serial.println("Right");
    dt = 2000;
  }
  else if (gy > 20000)
  {
    Serial.println("Down");
    dt = 2000;
  }
  else if (gy < -20000)
  {
    Serial.println("Up");
    dt = 2000;
  }
  else
  {
    dt = 100;
  }

  return 1;
}