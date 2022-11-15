#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define SSID "AirPort"
#define PSK "aaabbbccc"
#define NTPSERVER "ntp6.aliyun.com"
#define SHANGHAI 28800

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPSERVER);
ESP8266WiFiMulti WiFiMulti;

String weekDays[7] = {"日", "一", "二", "三", "四", "五", "六"};

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, 0, 2);

void connect_to_ap();

void setup(void)
{
  if (!u8g2.begin())
  {
    Serial.println("u8g2 failed.");
    for (;;)
      ;
  }
  u8g2.enableUTF8Print(); // enable UTF8 support for the Arduino print() function
                          // 串口通信设置
  Serial.begin(115200);
  // IIC 通信设置
  Wire.begin(2, 0);

  connect_to_ap();
  timeClient.begin();
  timeClient.setTimeOffset(28800);

  u8g2.setFont(u8g2_font_unifont_t_chinese2); // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFontDirection(0);
}

void loop(void)
{
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  // String currentMonthName = months[currentMonth - 1];
  int currentYear = ptm->tm_year + 1900;

  u8g2.firstPage();
  do
  {
    u8g2.setCursor(0, 15);
    u8g2.printf("星期%s", weekDay);
    u8g2.setCursor(0, 40);
    u8g2.print("你好，我的世界～"); // Chinese "Hello World"
    // u8g2.print("こんにちは世界");		// Japanese "Hello World"
  } while (u8g2.nextPage());
  delay(1000);
}

// 连接到AP
// 并将信号灯设置闪烁
#define DELAY 200

void connect_to_ap()
{
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
