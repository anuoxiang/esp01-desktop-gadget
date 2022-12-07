#ifndef _Mpu_H_
#define _Mpu_H_

#include <MPU6050.h>
#include <Wire.h>
#include "util.h"
#include "display.h"

void mpu_setup();
ProcessInfo mpu_main(void);

#endif
