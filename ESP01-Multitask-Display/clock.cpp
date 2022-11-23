#include "clock.h"

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
