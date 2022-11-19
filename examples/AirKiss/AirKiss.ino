#include <ESP8266WiFi.h>
//控制器led灯
int led = 2 ;
void setup() {
  //绑定波特率端口
  Serial.begin(115200);
  //初始化LED灯
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  if (!autoConfig()) {
    smartConfig();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

//*******************************************************   WIFI 配网 ***************************************************
//ap 配网
void smartConfig() {
  WiFi.mode(WIFI_STA);
  Serial.println("\r\n开始配网 Smartconfig...");
  WiFi.beginSmartConfig();
  while (1) {
    Serial.println(".");
    // 等待时间
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
    digitalWrite(led, HIGH);
    if (WiFi.smartConfigDone()) { // 配网成功
      Serial.println("SmartConfig Success");
      digitalWrite(led, LOW);
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      Serial.print("LocalIP:");
      Serial.print(WiFi.localIP());
      Serial.print(" ,GateIP:");
      Serial.println(WiFi.gatewayIP());

      break;
    }
  }
}


//自动连接
bool autoConfig() {
  WiFi.begin();
  //如果觉得时间太长可改
  for (int i = 0; i < 20; i++) {
    int wstatus = WiFi.status();

    if (wstatus == WL_CONNECTED) {
      digitalWrite(led, LOW);
      Serial.println("WIFI SmartConfig Success");
      Serial.printf("SSID:%s", WiFi.SSID().c_str());
      Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
      Serial.print("LocalIP:");
      Serial.print(WiFi.localIP());
      Serial.print(" ,GateIP:");
      Serial.println(WiFi.gatewayIP());

      return true;
    } else {
      Serial.println("WIFI AutoConfig Waiting......");
      delay(1000);
      digitalWrite(led, LOW);
      delay(1000);
      digitalWrite(led, HIGH);
    }
  }
  Serial.println("WIFI 配网" );
  return false;
}
