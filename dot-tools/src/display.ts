/**
 * Oled屏幕模拟器
 * Django 2022-11-16
 */

// 网格线与灯的宽度比例，1线5灯
const LINE_LIGHT_RATIO = 1 / 6;
// 网格线颜色
const LINE_COLOR: string = "rgb(128,128,128)";
const LINE_COLOR_2: string = "rgb(192,192,192)";
/**
 * 模拟OLED屏幕
 */
export class Display {
  // 网格线宽度
  public readonly lineWidth: number;
  // canvas.context
  public readonly ctx: CanvasRenderingContext2D;
  // 网格尺寸
  public readonly grid_width: number;

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
    // 网格中，像素点的间隔计算，以0.96" SSD1306 OLED 128*64 为例：
    // 像素宽度与间隔为5:1，即：像素宽度为5单位，间隔为1单位，
    // 需要绘制127条垂直线和63条水平线，每条水平线的宽度算法
    // 总宽度 = 128*(1-1/6)x + 127*(1/6)x => x = 总宽度 / [128*(5/6)+127(1/6)]
    // this.lineWidth = (cvs.width / width) * LINE_LIGHT_RATIO;
    // 调整cvs的宽和高，实质内部的像素可以有整数。
    this.lineWidth = (cvs.width / (128 * (1 - LINE_LIGHT_RATIO) + 127 * LINE_LIGHT_RATIO)) * LINE_LIGHT_RATIO;
    this.lineWidth = Math.floor(this.lineWidth);
    cvs.width = this.lineWidth * 5 * 128 + this.lineWidth * 127;
    cvs.height = cvs.width / (this.width / this.height);
    console.log(this.lineWidth, this.lineWidth * 5 * 128 + this.lineWidth * 127);

    this.ctx = cvs.getContext("2d")!;
    this.grid_width = cvs.width / width;
    console.log(cvs.width, cvs.height);

    // 画线

    this.ctx.fillStyle = "black";
    this.ctx.fillRect(0, 0, cvs.width, cvs.height);
    this.ctx.lineWidth = this.lineWidth;

    this.ctx.beginPath();
    this.ctx.strokeStyle = LINE_COLOR;
    for (let i = 0; i < this.width - 1; i++) {
      this.ctx.moveTo((1 + i) * this.grid_width, 0);
      this.ctx.lineTo((1 + i) * this.grid_width, cvs.height);
    }
    this.ctx.stroke();

    for (let i = 0; i < this.height - 1; i++) {
      this.ctx.beginPath();
      this.ctx.strokeStyle = this.dual_color && i === 15 ? LINE_COLOR_2 : LINE_COLOR;
      this.ctx.moveTo(0, (i + 1) * this.grid_width);
      this.ctx.lineTo(cvs.width, (i + 1) * this.grid_width);
      this.ctx.stroke();
    }
    console.log("??");
    cvs.addEventListener("click", ({ offsetX, offsetY }) => {
      console.log("click");
      // let x = offsetX
      // 喷涂这个像素点区域的颜色，注意：需要把网格线的宽度考虑进去。
      let x = Math.floor(offsetX / this.grid_width),
        y = Math.floor(offsetY / this.grid_width);
      this.ctx.fillStyle = this.color;
      this.ctx.fillRect(
        x * this.grid_width + (x === 0 ? 0 : this.lineWidth / 2),
        y * this.grid_width + (y === 0 ? 0 : this.lineWidth / 2),
        this.grid_width - (x === 0 ? this.lineWidth / 2 : this.lineWidth),
        this.grid_width - (y === 0 ? this.lineWidth / 2 : this.lineWidth)
      );
    });
  }

  // 响应鼠标的点击事件
}

// function clickDot(ctx:CanvasRenderingContext2D):
