#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define SSID "AirPort"
#define PSK "aaabbbccc"
#define NTPSERVER "ntp6.aliyun.com"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SHANGHAI 28800

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPSERVER);
ESP8266WiFiMulti WiFiMulti;

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // 串口通信设置
  Serial.begin(115200);
  // IIC 通信设置
  Wire.begin(2, 0);
  // 屏幕连接
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  connect_to_ap();
  timeClient.begin();
  timeClient.setTimeOffset(28800);
}

void loop() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  String currentMonthName = months[currentMonth - 1];
  int currentYear = ptm->tm_year + 1900;

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.printf("%d-%02d-%02d", currentYear, currentMonth, monthDay);

  display.setTextSize(4);
  display.setCursor(0, 16);
//  display.printf("%02d:%02d:%02d", currentHour, currentMinute, currentSecond);
  display.printf("%02d:%02d", currentHour, currentMinute);
  display.display();
  delay(1000);
}



// 连接到AP
// 并将信号灯设置闪烁
#define DELAY 200

void connect_to_ap() {
  // 连接
  Serial.println("Begin to connect to AP.");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("connect to ");
  display.print(SSID);
  display.display();
  // 载入（或者使用Smart Config) AP 账号
  WiFi.mode(WIFI_STA);
  //  WiFi.begin(SSID, PSK);
  WiFiMulti.addAP(SSID, PSK);
  /**
     直到连接到AP上，退出过程。
     如果太长时间没有连上Wifi，考虑是更换了场所，
     重新启动Smart Config 监听
  */
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
  }
  display.setCursor(0, 32);
  display.print("connected");
  display.display();
}
