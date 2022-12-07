#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "util.h"

#define SCREEN_WIDTH 128

extern uint8_t turn_to_screen, current_screen;
extern int16_t offsetX;
extern const uint8_t SCREEN_NUM;

ProcessInfo display_main();

/**
 * @brief 是否显示当前屏幕内容
 * @param display_num 程序所占用的屏幕编号
 * @return 是否显示
 */
bool is_show_me(uint8_t display_num);

/**
 * @brief 获得当前显示屏幕的偏移量
 * @param display_num 程序所占用的屏幕编号
 * @return 偏移量
 */
int16_t get_offset(uint8_t display_num);

#endif