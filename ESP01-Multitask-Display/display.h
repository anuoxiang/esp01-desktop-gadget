#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "util.h"

// 当前屏幕编号
uint8_t current_screen = 0;

// 控制屏幕输出的偏移量
int16_t offsetX = 0;

// 如果当前屏幕编号与需要转移的（目标）屏幕号不一致，则使用屏幕资源输出的时候需要
// 处理输出偏移量。

#endif