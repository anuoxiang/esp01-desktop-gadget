import { Box, Position } from "./gadgets";

// 元素类型
export enum ElementType {
  TEXT, //文本
  IMAGE, //图片
  ANIMATE, //动画
  DOTS, // 手绘点阵
}

/**
 * 点阵元素
 * 点阵的对象操作实体；目的是当形成对象后，用户可以很方便地通过拖拽的方式设计显示界面的布局，
 * 也可以通过单独输出逻辑控制，将屏幕中的元素与主程序解耦，实现更高程度的复用。
 *
 */
export class DotsElement {
  pos: Position;
  box: Box;

  // 文字
  text?: string;
  // 图片
  image?: Blob;
  // 动画
  animate?: Array<Blob>;
  // 绘制
  dots: Array<Position> = [];
  // 编号
  id: string;
  // 人工赋予的名称
  name: string = "";

  _type: ElementType = ElementType.DOTS;

  /**
   * 像素点元素
   * @param p1 坐标起点
   * @param p2 坐标终点
   * @param gridWidth 网格宽度
   * @param gridHeight 网格高度
   */
  constructor(p1: Position, p2: Position, public readonly gridWidth: number = 1, private dotColor: string = "#195BFF") {
    // 判断起点（坐标值小的）和重点
    this.pos = new Position(p1.X > p2.X ? p2.X : p1.X, p1.Y > p2.Y ? p2.Y : p1.Y);
    this.box = new Box(Math.abs(p1.X - p2.X), Math.abs(p1.Y - p2.Y));
    this.id = "";
  }

  /**
   * 对象类型
   */
  get type(): ElementType {
    return this._type;
  }

  /**
   * 修改元素类型
   * 修改后需要把内部清空
   */
  set type(newType: ElementType) {
    this._type = newType;
    this.dots = [];
    this.text = "";
    this.image = undefined;
    this.animate = [];
  }
  // 是否在元素的区域中科技以
  public here(_pos: Position): Boolean {
    return (
      _pos.X >= this.pos.X &&
      _pos.Y >= this.pos.Y &&
      _pos.X <= this.pos.X + this.box.width &&
      _pos.Y <= this.pos.Y + this.box.height
    );
  }

  /**
   * 点按元素上的一个坐标
   * @param pos 画布坐标（非网格坐标）
   */
  public click(pos: Position): void {
    // console.log(`relative pos:${pos.X - this.pos.X},${pos.Y - this.pos.Y}`);
    switch (this.type) {
      case ElementType.DOTS: // 手绘点阵对象，点击=绘制
        // 处理网格长宽，得到换算后的点阵坐标
        // 比对现有的点阵，是绘制还是擦除
        // 记录到this.dots中
        let offsetPos = new Position(pos.X - this.pos.X, pos.Y - this.pos.Y, this.gridWidth, Math.floor);

        let index = this.dots.findIndex((d) => d.equal(offsetPos));
        if (index >= 0) {
          this.dots.splice(index, 1);
        } else {
          this.dots.push(offsetPos);
        }
        // console.table(this.dots);
        break;
      default:
        throw new Error("unimplement code .");
    }
  }

  /**
   * 绘制元素到画布上
   * 保存元素的内容，并根据内容绘制到画布上。
   * @param ctx CanvasContext
   */
  public draw(ctx: CanvasRenderingContext2D, lineWidth: number = 0): void {
    switch (this.type) {
      case ElementType.DOTS:
        for (let dot of this.dots) {
          ctx.fillStyle = this.dotColor;
          ctx.fillRect(
            dot.X + this.pos.X + (dot.X + this.pos.X === 0 ? 0 : lineWidth / 2),
            dot.Y + this.pos.Y + (dot.Y + this.pos.Y === 0 ? 0 : lineWidth / 2),
            this.gridWidth - (dot.X + this.pos.X === 0 ? lineWidth / 2 : lineWidth),
            this.gridWidth - (dot.Y + this.pos.Y === 0 ? lineWidth / 2 : lineWidth)
          );
        }
        break;
      default:
        throw new Error("unimplement code .");
    }
  }
}
