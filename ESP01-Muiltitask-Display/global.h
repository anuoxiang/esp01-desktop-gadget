#ifndef _UTILIT_H_
#define _UTILIT_H_
#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

#include <U8g2lib.h>

#define SSID "AirPort"
#define PSK "aaabbbccc"
#define NTPSERVER "ntp6.aliyun.com"
#define SHANGHAI 28800
#define DAY_LIGHT_OFFSET_SEC 0

#define countof(a) (sizeof(a) / sizeof(a[0]))
#define TIME_SLICE 50

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// 连接到AP
void connect_to_ap();

enum PSTATUS
{
  // 下个周期退出
  NEXT_TIME_TO_EXIT = -1,
  // 暂停
  PAUSE = 0,
  // 运行中
  RUNNING = 1
};

// 进程可以自行修改状态信息
typedef struct ProcessInfo
{
  ulong period;
  PSTATUS status;
};

/**
 * 控制屏幕显示的进程结构
 */
typedef struct Display
{
  // PID，系统自动分配
  unsigned short pid;
  // 程序指针，运行后返回当前进程的状态
  void (*run)(void);
};

// 进程结构
typedef struct Process : Display
{
  // PID，系统自动分配
  unsigned short pid;
  // 程序指针，运行后返回当前进程的状态
  ProcessInfo (*run)(void);
  // 运行周期，以毫秒为单位，要求每隔多少毫秒运行一次
  unsigned long period;
  // 运行状态
  PSTATUS status;
  // 最后运行时间
  unsigned long last_run_time_at = 0;
  // 优先级
  uint8_t priority = 0;
};
#endif