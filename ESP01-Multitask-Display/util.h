#ifndef _UTILIT_H_
#define _UTILIT_H_
#include <Arduino.h>

#define SCREEN_WIDTH 128

enum PSTATUS
{
  // 下个周期退出
  NEXT_TIME_TO_EXIT = -1,
  // 暂停
  PAUSE = 0,
  // 运行中
  RUNNING = 1
};

// 进程可以自行修改状态信息
typedef struct ProcessInfo
{
  ulong period;
  PSTATUS status;
};

// 进程结构
typedef struct Process
{
  // PID，系统自动分配
  unsigned short pid;
  // 程序指针，运行后返回当前进程的状态
  ProcessInfo (*run)(void);
  // 运行周期，以毫秒为单位，要求每隔多少毫秒运行一次
  unsigned long period;
  // 运行状态
  PSTATUS status;
  // 最后运行时间
  unsigned long last_run_time = 0;
  // Last time run at = millis()
  unsigned long ltra = 0;
  // 优先级
  uint8_t priority = 0;
};

/**
 * 切换屏幕的全局静态变量
 * 实际由屏幕控制程序操控当前屏幕
 * 屏幕的编号0,1,2...
 */
static uint8_t turn_to_screen = 0;

const uint8_t SCREEN_NUM = 2;

#endif