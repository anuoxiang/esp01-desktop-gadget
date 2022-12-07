#include "clock.h"
#include "display.h"

WiFiUDP ntpUDP;
ESP8266WiFiMulti WiFiMulti;
String weekDays[7] = {"日", "一", "二", "三", "四", "五", "六"};

bool b_clock_setup = false, flash = true;
uint8_t width, x_pos, colon_width;

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

ProcessInfo clock_main(void)
{
  if (!b_clock_setup)
    clock_setup();

  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  // Serial.printf("%d-%d %d", timeinfo->tm_hour);
  Serial.print(asctime(timeinfo));
  int currentYear = timeinfo->tm_year + 1900;
  int currentMonth = timeinfo->tm_mon + 1;
  int monthDay = timeinfo->tm_mday;
  String weekDay = weekDays[timeinfo->tm_wday];
  int currentHour = timeinfo->tm_hour;
  int currentMinute = timeinfo->tm_min;

  // u8g2.clearBuffer();
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
  u8g2.drawUTF8(x_pos, 53, text);

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

  ProcessInfo result;

  result.period = 1000;
  result.status = RUNNING;
  return result;
}