/**
 * ESP01 文件读写实验
 * Django 2022-11-08
 * Reference:  https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#sdfs-and-sd
 *
 */

#include <FS.h>
#include <string.h>

#define FS SPIFFS
int a = 1;
int somelongname = 2;
double c = 3;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Started");
  Serial.printf("ChipID: %s\n", String((uint64_t)ESP.getChipId()));

  if (!FS.begin())
  {
    Serial.println("FS mount failed.");
    return;
  }
  // if (!FS.format())
  // {
  //   Serial.println("FS format failed.");
  //   return;
  // }

  Serial.println("FS mounted.");

  FSInfo info;
  FS.info(info);
  Serial.printf("total size: %d\n", info.totalBytes);
  Serial.flush();

  // 写入一个文件，写入几个ASCII字符
  // File f = FS.open("/hello.txt", "w");
  // if (!f)
  // {
  //   Serial.println("file create failed.");
  // }
  // f.write("Hello world! @Django 2022-11-11");
  // f.close();

  Dir dir = FS.openDir("/");
  while (dir.next())
  {
    Serial.println(dir.fileName());
    if (dir.fileSize())
    {
      File f = dir.openFile("r");
      Serial.println(f.size());

      uint16_t len = f.available();
      Serial.println(f.readStringUntil('\0'));
    }
  }
  Serial.flush();
  FS.end();
  // 并且
}

void loop()
{
  // put your main code here, to run repeatedly:
}
