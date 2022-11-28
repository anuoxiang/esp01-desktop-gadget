#ifndef _Mpu_H_
#define _Mpu_H_

#include <MPU6050.h>
#include <Wire.h>

void mpu_setup();
int8_t mpu_main(void);

#endif
