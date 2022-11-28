#ifndef _Clock_H_
#define _Clock_H_

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define SSID "AirPort"
#define PSK "aaabbbccc"
#define NTPSERVER "ntp6.aliyun.com"
#define SHANGHAI 28800
#define DAY_LIGHT_OFFSET_SEC 0

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// 是否完成设置
// static bool b_clock_setup = false;

// 设置过程
void clock_setup();

// 连接到AP
void connect_to_ap();

// 主过程
int8_t clock_main();

#endif