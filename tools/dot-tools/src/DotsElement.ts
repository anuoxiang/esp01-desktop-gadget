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
  animate?: [];
  // 绘制
  dots?: [][];
  // 编号
  id: string;
  // 人工赋予的名称
  name: string = "";

  constructor(p1: Position, p2: Position) {
    // 判断起点（坐标值小的）和重点
    this.pos = new Position(p1.X > p2.X ? p2.X : p1.X, p1.Y > p2.Y ? p2.Y : p1.Y);
    this.box = new Box(Math.abs(p1.X - p2.X), Math.abs(p1.Y - p2.Y));
    this.id = "";
  }

  /**
   * 对象类型
   */
  get type(): ElementType {
    return this.text
      ? ElementType.TEXT
      : this.image
      ? ElementType.IMAGE
      : this.animate
      ? ElementType.ANIMATE
      : ElementType.DOTS;
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
}
