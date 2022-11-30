/**
 * Oled屏幕模拟器
 * Django 2022-11-16
 */

import { DotsElement } from "./DotsElement";
import { Position } from "./gadgets";

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
  // 当前激活状态的元素对象
  public selectedElement: number = -1;
  // 在画布上的所有元素
  public elements: DotsElement[] = [];

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
    // this.lineWidth = Math.floor(this.lineWidth);
    // cvs.width = this.lineWidth * 5 * 128 + this.lineWidth * 127;
    // cvs.height = cvs.width / (this.width / this.height);
    //console.log(this.lineWidth, this.lineWidth * 5 * 128 + this.lineWidth * 127);

    this.ctx = cvs.getContext("2d")!;
    this.grid_width = cvs.width / width;
    console.log(cvs.width, cvs.height);
    console.log("??");
    this.initGrid();
    this.bindMouse();

    // cvs.addEventListener("click", ({ offsetX, offsetY }) => {
    //   console.log("click");
    //   // let x = offsetX
    //   // 喷涂这个像素点区域的颜色，注意：需要把网格线的宽度考虑进去。
    //   let x = Math.floor(offsetX / this.grid_width),
    //     y = Math.floor(offsetY / this.grid_width);
    //   this.ctx.fillStyle = this.color;
    //   this.ctx.fillRect(
    //     x * this.grid_width + (x === 0 ? 0 : this.lineWidth / 2),
    //     y * this.grid_width + (y === 0 ? 0 : this.lineWidth / 2),
    //     this.grid_width - (x === 0 ? this.lineWidth / 2 : this.lineWidth),
    //     this.grid_width - (y === 0 ? this.lineWidth / 2 : this.lineWidth)
    //   );
    // });
  }

  // 是否进入检测拖动模式，即：mousedown之后就是mousemove
  private detectMouse: Boolean = false;

  // 是否为单击模式，当出现dragMode时，mouseup同时也会触发click，使其无效
  private singleClick: Boolean = true;

  // 绑定鼠标的拖动与点击操作
  private bindMouse() {
    // 点按并拖动的基础点
    let basePoint: Position;
    let lastOffset: Position;
    // 被当前拖动的元素
    let movingElement: number = -1;

    // 鼠标按下
    this.cvs.addEventListener("mousedown", ({ offsetX, offsetY }) => {
      this.detectMouse = true;
      basePoint = new Position(offsetX, offsetY, this.grid_width);
      lastOffset = basePoint;
      // 判断当前坐标有没有元素
      for (let element of this.elements) {
        if (element.here(basePoint)) {
          movingElement = this.elements.indexOf(element);
          break;
        }
      }
      if (movingElement < 0) {
        // 没有任何内容，可以开始建立元素框
        this.selectedElement = -1;
      } else {
        // 表示选中这个元素，并开始拖动
        console.log("lllll", movingElement);
      }
    });
    // 鼠标松开
    this.cvs.addEventListener("mouseup", () => {
      this.detectMouse = false;
      setTimeout(() => {
        this.singleClick = true;
      }, 1); // 统一瞬间触发，难保证click与up谁先谁后；
      // basePoint to lastOffset 形成一个元素区域
      // 判断是新建一个元素，还是拖动的元素
      if (movingElement < 0) {
        if (basePoint.X === lastOffset.X || basePoint.Y === lastOffset.Y) {
          // 在头一条直线上的，没有面积，所以不做任何动作（仅重绘）
        } else {
          let element = new DotsElement(basePoint, lastOffset);
          this.elements.push(element);
        }
        this.reDraw();
      } else {
        // 移动元素结束
        movingElement = -1;
      }
    });
    // 鼠标移动
    this.cvs.addEventListener("mousemove", ({ offsetX, offsetY }) => {
      if (!this.detectMouse) return;
      this.singleClick = false;

      let offset = new Position(offsetX, offsetY, this.grid_width);
      // 只有当移动超过一个方格的时候才有必要重绘
      if (lastOffset?.X == offset.X && lastOffset?.Y == offset.Y) return;

      // 检测是拖动还是框选
      if (movingElement < 0) {
        this.reDraw();
        let rect = new Path2D();
        rect.rect(basePoint.X, basePoint.Y, offset.X - basePoint.X, offset.Y - basePoint.Y);
        this.ctx.strokeStyle = "#fe9901";
        this.ctx.lineWidth = 1;
        this.ctx.stroke(rect);
      } else {
        this.selectedElement = movingElement;
        this.reDraw();
        this.elements[this.selectedElement].pos = new Position(
          this.elements[this.selectedElement].pos.X - (lastOffset.X - offset.X),
          this.elements[this.selectedElement].pos.Y - (lastOffset.Y - offset.Y)
        );
        let rect = new Path2D();
        rect.rect(
          this.elements[this.selectedElement].pos.X,
          this.elements[this.selectedElement].pos.Y,
          this.elements[this.selectedElement].box.width,
          this.elements[this.selectedElement].box.height
        );
        this.ctx.strokeStyle = "#fe9901";
        this.ctx.lineWidth = 1;
        this.ctx.stroke(rect);
      }

      lastOffset = offset;
    });
    // 鼠标单击（拖动或者单击，只能出现一个动作）
    this.cvs.addEventListener("click", ({ offsetX, offsetY }) => {
      if (!this.singleClick) {
        this.singleClick = true;
        return;
      }
      console.log(offsetX, offsetY);
    });
  }

  public reDraw(): void {
    this.initGrid();
    this.drawElements();
  }

  // 初始化网格
  public initGrid(): void {
    // 画线
    this.ctx.fillStyle = "black";
    this.ctx.fillRect(0, 0, this.cvs.width, this.cvs.height);
    this.ctx.lineWidth = this.lineWidth;

    this.ctx.beginPath();
    this.ctx.strokeStyle = LINE_COLOR;
    for (let i = 0; i < this.width - 1; i++) {
      this.ctx.moveTo((1 + i) * this.grid_width, 0);
      this.ctx.lineTo((1 + i) * this.grid_width, this.cvs.height);
    }
    this.ctx.stroke();

    for (let i = 0; i < this.height - 1; i++) {
      this.ctx.beginPath();
      this.ctx.strokeStyle = this.dual_color && i === 15 ? LINE_COLOR_2 : LINE_COLOR;
      this.ctx.moveTo(0, (i + 1) * this.grid_width);
      this.ctx.lineTo(this.cvs.width, (i + 1) * this.grid_width);
      this.ctx.stroke();
    }
  }

  // 绘制所有元素
  public drawElements() {
    // 除了当前选中的不画
    for (let element of this.elements) {
      if (this.selectedElement == this.elements.indexOf(element)) continue;
      let rect = new Path2D();
      rect.rect(element.pos.X, element.pos.Y, element.box.width, element.box.height);
      this.ctx.strokeStyle = "#fe9901";
      this.ctx.lineWidth = 1;
      this.ctx.stroke(rect);
    }
  }
}
