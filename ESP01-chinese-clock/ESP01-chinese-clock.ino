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

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, 0, 2);
// Full buffer
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, 0, 2);

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
}

u8g2_uint_t width, x_pos, colon_width;
bool flash = true;
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

  u8g2.clearBuffer();
  //  u8g2_font_10x20_tn
  u8g2.setFont(u8g2_font_unifont_t_chinese2); // use chinese2 for all the glyphs of "你好世界"

  // 处理剧中问题
  char text[30];
  snprintf(text, sizeof(text), "%d-%02d-%02d %s", currentYear, currentMonth, monthDay, weekDay);
  width = u8g2.getUTF8Width(text);
  u8g2.setCursor((u8g2.getDisplayWidth() - width) / 2, 15);
  u8g2.printf(text);

  u8g2.setFont(u8g2_font_mystery_quest_48_tn);
  snprintf(text, sizeof(text), "%02d:%02d", currentHour, currentMinute);
  width = u8g2.getUTF8Width(text);
  x_pos = (u8g2.getDisplayWidth() - width) / 2;
  // u8g2.setCursor(x_pos, 53);
  // u8g2.printf(text);
  u8g2.drawUTF8(x_pos, 53, text);

  // flash是否清空那个":"，起始位置+小时长度 到 冒号宽度 42像素高清空
  if (flash)
  {
    flash = false;
    snprintf(text, sizeof(text), "%02d", currentHour);
    width = u8g2.getUTF8Width(text);
    colon_width = u8g2.getUTF8Width(":");
    u8g2.setDrawColor(0);
    u8g2.drawBox(x_pos + width, 16, colon_width, 48);
    u8g2.setDrawColor(1);
  }
  else
  {
    flash = true;
  }
  // 绘制分页，点形
  // u8g2.drawDisc(50, 60, 3, U8G2_DRAW_ALL);
  // u8g2.drawCircle(78, 60, 3, U8G2_DRAW_ALL);

  // 绘制分页，条形
  u8g2.drawBox(0, 57, 64, 6);
  u8g2.setFont(u8g2_font_squeezed_b6_tn);
  u8g2.setDrawColor(0);
  u8g2.drawUTF8(26, 63, "1");

  u8g2.setDrawColor(1);
  u8g2.drawFrame(64, 57, 64, 6);
  // u8g2.drawUTF8(96, 63, "2");
  u_long begin = millis();
  u8g2.sendBuffer();
  // Serial.printf("%d\n", millis() - begin);

  // 滚动动画
  u8g2.setFont(u8g2_font_mystery_quest_48_tn);
  snprintf(text, sizeof(text), "%02d:%02d", currentHour, currentMinute);
  width = u8g2.getUTF8Width(text);
  x_pos = (u8g2.getDisplayWidth() - width) / 2;
  int16_t offset = 0;
  do
  {
    u8g2.clearBuffer();
    u8g2.drawUTF8(x_pos - offset, 53, text);
    u8g2.updateDisplayArea(0, 2, 16, 5);
    offset += 3;
  } while (x_pos - offset + width > 0);

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
