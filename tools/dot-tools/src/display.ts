/**
 * Oled屏幕模拟器
 * Django 2022-11-16
 */

import { generateControlPanelForm } from "./controlPanel";
import { DotsElement, ElementType } from "./dotsElement";
import { Position } from "./gadgets";

// 网格线与灯的宽度比例，1线5灯
const LINE_LIGHT_RATIO = 1 / 6;

// 配色
const COLOR_BACKGROUND = "black";
const COLOR_LINE = "#888888";
const COLOR_LINE2 = "#CCCCCC";
// https://next.startdt.com/img/charts/%E9%85%8D%E8%89%B2%E6%96%B9%E6%A1%88-%E6%A0%87%E5%87%86%E8%89%B2.png
const COLOR_ELEMENT_BOX = "#FBC228";
const COLOR_CURRENT_ELEMENT_BOX = "#01CAFF";

// Display支持的事件类型
const EVENT_NAME = ["newelement", "selectelement", "delelement"];

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
    public readonly color: string = "blue",
    public readonly controlPanel: HTMLElement
  ) {
    // 处理画布与屏幕的对应关系
    // 网格中，像素点的间隔计算，以0.96" SSD1306 OLED 128*64 为例：
    // 像素宽度与间隔为5:1，即：像素宽度为5单位，间隔为1单位，
    // 需要绘制127条垂直线和63条水平线，每条水平线的宽度算法
    // 总宽度 = 128*(1-1/6)x + 127*(1/6)x => x = 总宽度 / [128*(5/6)+127(1/6)]
    // this.lineWidth = (cvs.width / width) * LINE_LIGHT_RATIO;
    // 调整cvs的宽和高，实质内部的像素可以有整数。
    this.lineWidth = 1; //(cvs.width / (128 * (1 - LINE_LIGHT_RATIO) + 127 * LINE_LIGHT_RATIO)) * LINE_LIGHT_RATIO;
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
    this.bindControlPanel();
  }

  /**
   * 绑定生成控制界面s
   */
  private bindControlPanel(): void {
    generateControlPanelForm(
      this.controlPanel,
      (type: ElementType) => {
        this.elements[this.selectedElement].type = type;
      },
      (content: string) => {
        this.elements[this.selectedElement].type = ElementType.TEXT;
        this.elements[this.selectedElement].text = content;
      }
    );
  }

  /**
   * 触发元素被选中时，内部（必要）事件
   */
  private internalOnSelected() {
    // 操控界面解锁
    let eleType = this.controlPanel.querySelector<"select">("select")!;
    eleType.value = this.elements[this.selectedElement].type;
    eleType.disabled = false;
    let txtContent = this.controlPanel.querySelector<"input">("input")!;
    txtContent.value = this.elements[this.selectedElement].text;
    txtContent.disabled = false;

    // 触发选中事件
    let promises = this.events
      .filter((e) => e.name === "selectelement")
      .map((e) => e.callback(this.elements[this.selectedElement]));

    return Promise.all(promises);
  }

  // 是否进入检测拖动模式，即：mousedown之后就是mousemove
  private detectMouse: Boolean = false;

  // 是否为单击模式，当出现dragMode时，mouseup同时也会触发click，使其无效
  private singleClick: Boolean = true;

  // 绑定鼠标的拖动与点击操作
  private bindMouse(): void {
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
      movingElement = -1;
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
        // 有选择s
      }
    });
    // 鼠标松开
    this.cvs.addEventListener("mouseup", () => {
      this.detectMouse = false;
      // 根据标准，mouseup是先于click触发，所以这里无需处理this.singleClick = true;

      if (movingElement < 0) {
        this.reDraw();
        if (basePoint.X === lastOffset.X || basePoint.Y === lastOffset.Y) {
          // 在头一条直线上的，没有面积，所以不做任何动作（仅重绘）
        } else {
          let element = new DotsElement(basePoint, lastOffset, this.grid_width);
          this.elements.push(element);
          this.selectedElement = this.elements.length - 1;
          this.drawSelected();
          // 触发选中事件
          this.internalOnSelected();
        }
      } else {
        // 移动元素结束
        // movingElement = -1;
      }
    });

    // 鼠标移动
    this.cvs.addEventListener("mousemove", ({ offsetX, offsetY }) => {
      if (!this.detectMouse) return;
      this.singleClick = false;
      // 表示选中这个元素，并开始拖动
      this.selectedElement = movingElement;
      let offset = new Position(offsetX, offsetY, this.grid_width);
      // 只有当移动超过一个方格的时候才有必要重绘
      if (lastOffset?.X == offset.X && lastOffset?.Y == offset.Y) return;

      // 检测是拖动还是框选
      if (movingElement < 0) {
        this.reDraw();
        let rect = new Path2D();
        rect.rect(basePoint.X, basePoint.Y, offset.X - basePoint.X, offset.Y - basePoint.Y);
        this.ctx.strokeStyle = COLOR_CURRENT_ELEMENT_BOX;
        this.ctx.lineWidth = 1;
        this.ctx.stroke(rect);
      } else {
        this.selectedElement = movingElement;
        this.reDraw();
        this.elements[this.selectedElement].pos = new Position(
          this.elements[this.selectedElement].pos.X - (lastOffset.X - offset.X),
          this.elements[this.selectedElement].pos.Y - (lastOffset.Y - offset.Y)
        );
        this.drawSelected();
        // 触发选中事件
        this.internalOnSelected();
      }

      lastOffset = offset;
    });

    // 鼠标单击（拖动或者单击，只能出现一个动作）
    this.cvs.addEventListener("click", ({ offsetX, offsetY }) => {
      if (!this.singleClick) {
        // 拖动后阻断信号，下一次如果没有拖动的单击鼠标则表示单击
        this.singleClick = true;
        return;
      }

      // 判断两次点击同一个元素，则是修改元素的绘图
      if (this.selectedElement >= 0 && this.selectedElement == movingElement) {
        // 在已经被选中的前提下，又一次被单击选择
        this.elements[this.selectedElement].click(new Position(offsetX, offsetY));
        this.reDraw();
        this.drawSelected();
      } else if (movingElement >= 0) {
        this.selectedElement = movingElement;
        this.reDraw();
        this.drawSelected();
        // 触发选中事件
        this.internalOnSelected();
      }
    });
  }

  // 重绘
  public reDraw(): void {
    this.initGrid();
    this.drawElements();
  }

  // 初始化网格
  public initGrid(): void {
    // 画线
    this.ctx.fillStyle = COLOR_BACKGROUND;
    this.ctx.fillRect(0, 0, this.cvs.width, this.cvs.height);
    this.ctx.lineWidth = this.lineWidth;

    this.ctx.beginPath();
    this.ctx.strokeStyle = COLOR_LINE;
    for (let i = 0; i < this.width - 1; i++) {
      this.ctx.moveTo((1 + i) * this.grid_width, 0);
      this.ctx.lineTo((1 + i) * this.grid_width, this.cvs.height);
    }
    this.ctx.stroke();

    for (let i = 0; i < this.height - 1; i++) {
      this.ctx.beginPath();
      this.ctx.strokeStyle = this.dual_color && i === 15 ? COLOR_LINE2 : COLOR_LINE;
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
      this.ctx.strokeStyle = COLOR_ELEMENT_BOX;
      this.ctx.lineWidth = 1;
      this.ctx.stroke(rect);
      element.draw(this.ctx, this.lineWidth);
    }
  }

  public drawSelected() {
    if (this.selectedElement < 0) return;
    let element = this.elements[this.selectedElement];
    let rect = new Path2D();
    rect.rect(element.pos.X, element.pos.Y, element.box.width, element.box.height);
    this.ctx.strokeStyle = COLOR_CURRENT_ELEMENT_BOX;
    this.ctx.lineWidth = 1;
    this.ctx.stroke(rect);
    element.draw(this.ctx, this.lineWidth);
  }

  /**
   * Display 事件
   * 1. 产生一个新的元素 - newelement
   * 2. 一个元素被选中 - selectelement
   * 3. 元素被删除 - delelement
   */

  private events: Array<{ name: string; callback: Function }> = [];

  /**
   * 绑定事件响应
   * @param eventName 绑定的事件名称
   * @param callback 事件对应的处理函数
   */
  public addEventListener(eventName: string, callback: Function): void {
    if (EVENT_NAME.indexOf(eventName) < 0) throw new Error("Not support event name");
    this.events.push({ name: eventName, callback });
  }
}
