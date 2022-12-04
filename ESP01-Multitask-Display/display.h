#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "util.h"

// 如果当前屏幕编号与需要转移的（目标）屏幕号不一致，则使用屏幕资源输出的时候需要
// 处理输出偏移量。

// extern uint8_t current_screen;

ProcessInfo display_main(void);

#endif