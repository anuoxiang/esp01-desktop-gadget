/**
 * 座标点位置对象（只有XY）
 */
export class Position {
  x: number;
  y: number;
  constructor(_x: number, _y: number) {
    this.x = _x;
    this.y = _y;
  }
}

/**
 * 盒子对象（只有长宽）
 */
export class Box {
  width: number;
  height: number;
  constructor(_w: number, _h: number) {
    this.width = _w;
    this.height = _h;
  }
}

// 元素类型
export enum ElementType {
  TEXT, //文本
  IMAGE, //图片
  ANIMATE, //动画
  DOTS, // 手绘点阵
}

/**
 * 点阵元素
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

  constructor(_x?: number, _y?: number, _w?: number, _h?: number) {
    this.pos = new Position(_x ?? 0, _y ?? 0);
    this.box = new Box(_w ?? 0, _h ?? 0);
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
}
