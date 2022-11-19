/**
 * ESO91 多任务调度实验
 * Django 2022-11-12
 * 主题：在单核单线程的芯片中执行多任务的操作
 * 目标：
 * 1. 多个输入设备的检测，如陀螺仪、文件修改记录
 * 2. 执行跨多个时间片的任务，控制定时结束的任务（如果有可能）
 * 3. 管理任务的启动、执行、撤离
 *
 * 本实例：
 * 1. 更新NTP时间，并把时间显示到OLED上
 * 2. 每过一个时间间隔，将屏幕滚动显示
 */

// MPU i2c address 0x68

#define countof(a) (sizeof(a) / sizeof(a[0]))

// using RUN = std::function<void()>;
// 线程运行状态
enum PSTATUS {
  // 下个周期退出
  NEXT_TIME_TO_EXIT = -1,
  // 暂停
  PAUSE = 0,
  // 运行中
  RUNNING = 1
};

// 进程结构
typedef struct Process {
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
  // 下一个任务
  Process *next = NULL;
};

// 根线程
Process *first_procerss;

// 启动新的线程
uint8_t start_new_process(ulong period, PSTATUS status, void(*run));

// 每次都要使用的芯片启动时间，用于计算是否有程序运行超时
static ulong mils;

// 系统进程（啥也不做）
int8_t system() {
  delay(10);
  Serial.println("System Process.(delay 10).");
  return 0;
};

void setup() {
  Serial.begin(115200);

  // 初始化
  first_procerss = new Process;
  first_procerss->pid = 0;
  first_procerss->period = 10;
  first_procerss->status = RUNNING;
  first_procerss->run = &system;
}

void loop() {
  mils = millis();
  Process *p = first_procerss;
  // 循环所有在内存的指针
  // 管理任务的主程序，loop() 永远不会停
  while (p != NULL) {
    if (p->last_run_time == 0) {
    }
  }
}

/**
 * @brief 启动一个新的线程
 * @param period 运行周期
 * @param status 运行的默认状态
 * @param run 程序入口
 * @returns pid 进程编号
 */
uint8_t start_new_process(ulong period, PSTATUS status, int8_t (*run)()) {
  // pid的任务管理
  // 从第一个任务开始找到找到最后一个任务（节点）
  Process *p = first_procerss;
  while (p->next != NULL)
    p = p->next;

  // 生成新的pid，挂载到任务链条上
  Process *new_process = new Process;
  new_process->pid = p->pid + 1;
  new_process->period = period;
  new_process->run = run;
  new_process->status = status;
  new_process->next = NULL;

  // 挂载
  p->next = new_process;
  // 任务的pid
  return new_process->pid;
}
