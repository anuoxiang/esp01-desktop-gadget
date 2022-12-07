#include "display.h"

/**
 * 切换屏幕的全局变量
 * 实际由屏幕控制程序操控当前屏幕
 * 屏幕的编号1,2...
 */
uint8_t turn_to_screen = 1,
        current_screen = 1;

/**
 * 控制屏幕输出的偏移量
 * 小于零是向左移动，
 * 大于零是向右移动
 * 移动截至 = (current_screen - turn_to_screen) * SCREEN_WIDTH
 */
int16_t offsetX = 0;

// 一共有多少个屏幕
const uint8_t SCREEN_NUM = 2;

/**
 * 检测是否有滚动屏幕的操作
 * 是否运动到位置？设定当前屏幕到滚动目标，否则就设置偏移量；
 */
ProcessInfo display_main(void)
{
  if (turn_to_screen != current_screen)
  {
    // 判断OffsetX是否移动到目标位置上，即停止滚动动画
    if (turn_to_screen > current_screen)
    {
      // 左向移动，offsetX = 128 -> 0
      Serial.print("turn left:");
      offsetX--;
    }
    else
    {
      // 右向移动，offsetX = 0 -> 128
      Serial.print("turn right:");
      offsetX++;
    }
    Serial.println(offsetX);
    if (offsetX == 0 || offsetX == SCREEN_WIDTH)
    {
      Serial.println("done");
      // 如果移动到了临界点，说明已经移动到既定位置
      current_screen = turn_to_screen;
    }
  }

  ProcessInfo result;
  result.period = 50;
  result.status = RUNNING;
  return result;
}

/**
 * @brief 是否显示当前屏幕内容
 * @param display_num 程序所占用的屏幕编号
 * @return 是否显示
 */
bool is_show_me(uint8_t display_num)
{
  return display_num == turn_to_screen ||
         display_num == current_screen;
}

/**
 * @brief 获得当前显示屏幕的偏移量
 * @param display_num 程序所占用的屏幕编号
 * @return 偏移量
 * 偏移量：当前屏幕和滚动目标屏幕的中间位置为偏移量原点
 * 向左滚动：offsetX 从 SCREEN_WIDTH => 0
 * 向右滚动：offsetX 从 0 => SCREEN_WIDTH
 */
int16_t get_offset(uint8_t display_num)
{
  if (turn_to_screen == current_screen)
  {
    return 0;
  }
  else
  {
    // return offsetX - ((turn_to_screen - current_screen) * (display_num == current_screen ? 1 : -1) > 0 ? 128 : 0);
    if (turn_to_screen < current_screen)
    {
      if (display_num == current_screen)
      {
        return offsetX;
      }
      else
      {
        return offsetX - 128;
      }
    }
    else
    {
      if (display_num == current_screen)
      {
        return offsetX - 128;
      }
      else
      {
        return offsetX;
      }
    }
  }
}