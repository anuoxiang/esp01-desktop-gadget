#include <FS.h>
#include <string.h>

#define FS SPIFFS

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Started\n");
  
  if (!FS.begin(  )){ 
    Serial.println("FS mount failed.");
    return;
  }
  Serial.println("FS mounted.");

  // FSInfo info;
  // FS.info(info);
  // Serial.printf("total size: %d", info.totalBytes);
  // Serial.flush();

  // // 写入一个文件，写入几个ASCII字符
  // File f = FS.open("/hello.txt", "w");
  // if (!f)
  // {
  //   Serial.println("file create failed.");
  // }
  // 并且
}

void loop() {
  // put your main code here, to run repeatedly:

}
