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
  {
    Serial.println("Left");
    turn = 0;
    dt = MAX_DELAY;
  }
  else if (gx < -20000)
  {
    Serial.println("Right");
    turn = 1;
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