#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
MPU6050 accelgyro;
int16_t ax, ay, az, gx, gy, gz;
uint16_t dt = 3000;
void setup()
{
  Wire.begin(2, 0);
  Serial.begin(115200);
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop()
{
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //  Serial.printf("aX:%d\taY:%d\taZ:%d\tgX:%d\tgY:%d\tgZ:%d\n",ax,ay,az,gx,gy,gz);
  //    Serial.printf("aX:%d\taY:%d\n", gx, gy);

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

  delay(dt);
}
