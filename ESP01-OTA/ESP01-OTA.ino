#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

// ESP01 OTA 实验
// Django 2022-11-07
// Reference: https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html#id3

#ifndef SSID
#define SSID "AirPort"
#define PSK "aaabbbccc"
#endif

ESP8266WiFiMulti WiFiMulti;

//#define HOST "192.168.1.15"/
byte HOST[] = {192, 168, 1, 15};

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
  ota();
  WiFiClient http_client;
  if (!http_client.connect(HOST, 8080)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/";
  String postRequest = (String)("GET ") + url + " HTTP/1.1\r\n" +
                       "Content-Type: text/html;charset=utf-8\r\n" +
                       "Host: 192.168.1.5\r\n" +
                       "User-Agent: ESP-01\r\n" +
                       "Connection: Keep Alive\r\n\r\n";
  Serial.println(postRequest);
  http_client.print(postRequest);

  String resp = "";
  String line;
  do {
    line = http_client.readStringUntil('\n');
    Serial.println(line);
  } while (line.length() != 0);
  http_client.stop();

  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
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

void ota() {
  Serial.println("Try to OTA...");
  // ESPhttpUpdate.rebootOnUpdate(true);
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
  // 事件绑定
  ESPhttpUpdate.onStart(update_started);
  ESPhttpUpdate.onEnd(update_finished);
  ESPhttpUpdate.onProgress(update_progress);
  ESPhttpUpdate.onError(update_error);

  WiFiClient client;
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, "192.168.1.15", 8080, "/firmware.bin");
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n",
                    ESPhttpUpdate.getLastError(),
                    ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}


void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(uint32_t cur, uint32_t total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.println("\n===========!!!!!!!==========");
  Serial.printf("HTTP update fatal error code %d\n", err);
}
