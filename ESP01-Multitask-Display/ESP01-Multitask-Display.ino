/**
 * ESP01 多任务调度实验，显示屏版本
 * Django 2022-11-24
 * 主题：在单核的芯片中执行多任务。
 *      相比MPU的分支：多一个判断显示屏是否输出，并且将单链进程池管理改为数组
 */
#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include "clock.h"

// 计算数组尺寸
#define countof(a) (sizeof(a) / sizeof(a[0]))

// 显示屏类库
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, 0, 2);

enum PSTATUS
{
  // 下个周期退出
  NEXT_TIME_TO_EXIT = -1,
  // 暂停
  PAUSE = 0,
  // 运行中
  RUNNING = 1
};

// 进程结构
typedef struct Process
{
  // PID，系统自动分配
  unsigned short pid;
  // 程序指针
  int8_t (*run)();
  // 运行周期，以毫秒为单位，要求每隔多少毫秒运行一次
  unsigned long period;
  // 运行状态
  PSTATUS status;
  // 最后运行时间
  unsigned long last_run_time = 0;
  // 屏幕编号，0表示不需要使用屏幕
  uint8_t screen = 0;
};

// 线程数组，最大256个线程
Process processes[256];
// 线程数量
static uint8_t process_num = 0;

uint8_t start_new_process(ulong period, PSTATUS status, void(*run));

void setup()
{
  Serial.begin(115200);
  if (!u8g2.begin())
  {
    Serial.println("u8g2 failed.");
    for (;;)
      ;
  }
  u8g2.enableUTF8Print();
  Wire.begin(2, 0);
}

// 任务管理的轮询过程
void loop()
{
}

/**
 * @brief 启动一个新线程
 * @param period 运行周期
 * @param status 运行的默认状态
 * @param run 程序入口
 * @param screen 屏幕编号
 * @returns pid 进程编号
 */
uint8_t start_new_process(ulong period, PSTATUS status, int8_t (*run)(), uint8_t screen)
{
  process_num++;
  Process new_process;
  new_process.pid = process_num > 1 ? processes[process_num - 1].pid + 1 : 1;
  new_process.period = period;
  new_process.run = run;
  new_process.status = status;
  new_process.screen = screen;
  processes[process_num] = new_process;

  return new_process.pid;
}