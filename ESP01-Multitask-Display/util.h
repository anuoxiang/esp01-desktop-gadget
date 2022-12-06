#ifndef _UTILIT_H_
#define _UTILIT_H_
#include <Arduino.h>

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
#endif