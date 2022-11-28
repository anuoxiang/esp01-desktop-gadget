#include "clock.h"
WiFiUDP ntpUDP;
ESP8266WiFiMulti WiFiMulti;
String weekDays[7] = {"日", "一", "二", "三", "四", "五", "六"};

void clock_setup()
{
  b_clock_setup = true;
  connect_to_ap();
  configTime(SHANGHAI, DAY_LIGHT_OFFSET_SEC, NTPSERVER);
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  Serial.println(asctime(timeinfo));
  delay(1000);
}

void connect_to_ap()
{
  if (!b_clock_setup)
    clock_setup();
  // 连接
  Serial.println("Begin to connect to AP.");

  // 载入（或者使用Smart Config) AP 账号
  WiFi.mode(WIFI_STA);
  //  WiFi.begin(SSID, PSK);
  WiFiMulti.addAP(SSID, PSK);
  /**
     直到连接到AP上，退出过程。
     如果太长时间没有连上Wifi，考虑是更换了场所，
     重新启动Smart Config 监听
  */
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
  }
}

uint8_t clock_main()
{
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  // Serial.printf("%d-%d %d", timeinfo->tm_hour);
  // Serial.println(asctime(timeinfo));
  int currentYear = timeinfo->tm_year + 1900;
  int currentMonth = timeinfo->tm_mon + 1;
  int monthDay = timeinfo->tm_mday;
  String weekDay = weekDays[timeinfo->tm_wday];
  int currentHour = timeinfo->tm_hour;
  int currentMinute = timeinfo->tm_min;

  u8g2.beginSimple();

  return 1;
}