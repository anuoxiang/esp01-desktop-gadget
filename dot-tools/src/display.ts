/**
 * 模拟OLED屏幕
 */
export class Display {
  /**
   * 显示屏模拟
   * @param cvs Canvas 画布元素
   * @param width 屏幕像素宽度
   * @param height 屏幕像素高度
   * @param dual_color 是否双色显示
   * @param color 屏幕显示颜色
   */
  constructor(
    public readonly cvs: HTMLCanvasElement,
    public readonly width: number = 128,
    public readonly height: number = 64,
    public readonly dual_color: boolean = true,
    public readonly color: string = "blue"
  ) {
    // 处理画布与屏幕的对应关系
  }
}
