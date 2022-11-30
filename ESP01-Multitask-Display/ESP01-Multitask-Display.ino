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
#include "util.h"
#include "clock.h"
#include "mpu.h"
// 计算数组尺寸
#define countof(a) (sizeof(a) / sizeof(a[0]))
#define TIME_SLICE 50

// 显示屏类库
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, 0, 2);

// 线程数组，最大256个线程
static Process processes[256];
// 线程数量
static uint8_t process_num = 0;

uint8_t start_new_process(unsigned long period, ProcessInfo (*run)(void), uint8_t priority);

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

  start_new_process(1000, &clock_main, 1);
  start_new_process(100, &mpu_main, 1);
}

static ulong now = 0;
// 任务管理的轮询过程
void loop()
{
  // Serial.println("loop...");
  now = millis();
  uint8_t current = 0;
  // 待执行清单：循环线程池，找到到期（应该执行）的线程
  uint8_t task[256], max_task = 0;
  for (uint8_t i = 0; i < process_num; i++)
  {
    // 已经过了50天，millis重新从0开始，就会出现最后一次在“未来”的假象
    // 这时，需要重新复位所有进程池中的运行时间
    processes[i].last_run_time = now < processes[i].last_run_time ? 0 : processes[i].last_run_time;
    // Serial.printf("lrt:%d\t status:%d \t period:%d \t now:%d\t", processes[i].last_run_time, processes[i].status, processes[i].period, now);
    if (processes[i].status == RUNNING &
        processes[i].last_run_time + processes[i].period <= now)
    {
      task[max_task] = i;
      max_task++;
    }
  }

  // 找到待执行清单的最高优先级
  ulong delay_max = 0;
  uint8_t priority_max = 0;
  // Serial.printf("task: %d, process num:%d\n", max_task, process_num);
  if (max_task > 0)
  {
    current = task[0];
    // 找到最高优先级等待时间最久的进程
    for (uint8_t i = 1; i < max_task; i++)
      if (processes[current].period < processes[task[i]].period |
          (processes[current].period == processes[task[i]].period &
           processes[current].last_run_time + processes[current].priority <
               processes[task[i]].last_run_time + processes[task[i]].priority))
        current = task[i];

    // 记录执行进程的时间，并执行进程
    processes[current].last_run_time = now;
    ProcessInfo result = processes[current].run();
    processes[current].period = result.period;
    processes[current].status = result.status;

    // PSTATUS state = (PSTATUS)processes[current].run();

    // 修改状态，如果状态是待撤离，则撤离该进程
    if (processes[current].status == NEXT_TIME_TO_EXIT)
    {
      for (uint8_t i = current + 1; i < process_num; i++)
        processes[i - 1] = processes[i];
      process_num--;
    }
  }
  else
  {
    delay(TIME_SLICE);
  }
}

/**
 * @brief 启动一个新线程
 * @param period 运行周期
 * @param run 程序入口
 * @param priortiy 优先级
 * @returns pid 进程编号
 */
uint8_t start_new_process(ulong period, ProcessInfo(run)(void), uint8_t priortiy)
{

  Serial.println("start called");
  Process new_process;
  new_process.pid = process_num > 1 ? processes[process_num - 1].pid + 1 : 1;
  new_process.period = period;
  new_process.run = run;
  new_process.status = RUNNING;
  new_process.priority = priortiy;
  new_process.last_run_time = 0;
  processes[process_num] = new_process;
  process_num++;
  return new_process.pid;
}
