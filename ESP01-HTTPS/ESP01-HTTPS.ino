#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// ESP01 HTTPS 实验
// Django 2022-11-09
// Reference: https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html#id3

#ifndef SSID
#define SSID "AirPort"
#define PSK "aaabbbccc"
#endif

ESP8266WiFiMulti WiFiMulti;

// 启动串口输出调试信息
void setup() {
  Serial.begin(115200);
  Serial.println("Start!");

  //  pinMode(LED_BUILTIN, OUTPUT);/
  connect_to_ap();
}

// 主程序
void loop() {
  Serial.println("Get into main program.");
  Serial.print("ChipID:");
  Serial.print(String((uint64_t)ESP.getChipId()));
  Serial.print(" ,LocalIP:");
  Serial.print(WiFi.localIP());
  Serial.print(" ,GateIP:");
  Serial.println(WiFi.gatewayIP());


  Serial.println("Input a website addresss:");
  String site;
  do {
    site = Serial.readString();
    delay(100);
  } while (site == "");
  site.replace(String("\r"), emptyString);
  site.replace(String("\n"), emptyString);
  Serial.printf("http://%s/\n", site.c_str());
  open_url(site);
}

// 连接到AP
// 并将信号灯设置闪烁
#define DELAY 200
void connect_to_ap() {
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
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(DELAY);
    digitalWrite(LED_BUILTIN, LOW);
    delay(DELAY);
  }
}


void open_url(String url) {
  //  WiFiClient client;
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient http;

  if (!http.begin(*client, url)) {
    Serial.println("connection failed");
    return;
  }
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTPS] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
}
