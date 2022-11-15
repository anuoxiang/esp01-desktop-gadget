/*
  Django 测试OLED屏幕的刷新率，以在多任务的环境下控制输出频率

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 3 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
    {0b00000000, 0b11000000,
     0b00000001, 0b11000000,
     0b00000001, 0b11000000,
     0b00000011, 0b11100000,
     0b11110011, 0b11100000,
     0b11111110, 0b11111000,
     0b01111110, 0b11111111,
     0b00110011, 0b10011111,
     0b00011111, 0b11111100,
     0b00001101, 0b01110000,
     0b00011011, 0b10100000,
     0b00111111, 0b11100000,
     0b00111111, 0b11110000,
     0b01111100, 0b11110000,
     0b01110000, 0b01110000,
     0b00000000, 0b00110000};
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(2, 0);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("Allocation failed.");
    for (;;)
      ;
  }
  display.display();
}

#define XPOS 0 // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAY 2

void loop()
{
  // put your main code here, to run repeatedly:
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for (f = 0; f < NUMFLAKES; f++)
  {
    icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS] = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
  }
  long start = millis(), stop = 0;

  for (long i = 0; i < 1000; i++)
  {
    // start = millis();
    display.clearDisplay(); // Clear the display buffer
    // stop = millis();
    // Serial.printf("clear display: %d\n", stop - start);
    // Draw each snowflake:
    for (f = 0; f < NUMFLAKES; f++)
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
    }
    // start = millis();
    display.display(); // Show the display buffer on the screen
    delay(21);
    // stop = millis();
    // Serial.printf("display: %d\n", stop - start);
    // Then update coordinates of each flake...
    for (f = 0; f < NUMFLAKES; f++)
    {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height())
      {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS] = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
  delay(1000);
  stop = millis();
  Serial.printf("Frequency: %d fps\n", 1000 / ((stop - start) / 1000));
}
