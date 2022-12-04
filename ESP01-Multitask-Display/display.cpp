#include "display.h"

// 当前屏幕编号，从1开始
uint8_t current_screen = 1;

// 控制屏幕输出的偏移量
int16_t offsetX = 0;

/**
 * 检测是否有滚动屏幕的操作
 * 是否运动到位置？设定当前屏幕到滚动目标，否则就设置偏移量；
 */
ProcessInfo display_main(void)
{
  if (turn_to_screen != current_screen)
  {
    // 没有移动到位
    if ((turn_to_screen - current_screen) * SCREEN_WIDTH != offsetX)
      offsetX += turn_to_screen - current_screen;
    else
    {
      current_screen = turn_to_screen;
      offsetX = 0;
    }
  }

  ProcessInfo result;
  result.period = 50;
  result.status = RUNNING;
}