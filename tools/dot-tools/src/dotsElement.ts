import { Box, Position } from "./gadgets";

// 元素类型
export enum ElementType {
  TEXT, //文本
  IMAGE, //图片
  ANIMATE, //动画
  DOTS, // 手绘点阵
}

export interface ArtisicFont {
  text: string;
  fontName: string;
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

  // 文字，主要属性是文本内容和字体的名称
  aText?: ArtisicFont;

  // 文字
  // text: string = "";

  // 图片
  image?: Blob;

  // 动画
  animate?: Array<Blob>;

  // 不论是什么内容，最终都是对应到屏幕上的点阵
  private _dots: Array<Position> = [];

  // 编号
  id: string;

  // 人工赋予的名称
  name: string = "";

  // 当前元素的类型
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
   * 元素类型为只读
   * 根据其数据类型决定元素的类型，
   * 而不可以通过直接设置类型改变
   */
  // set type(newType: ElementType) {
  //   this._type = newType;
  //   this._dots = [];
  //   this.text = "";
  //   this.image = undefined;
  //   this.animate = [];
  // }

  /**
   * 设置字体
   */
  set text(aText: ArtisicFont | undefined) {
    this.aText = aText;
    // 将字体转换为点阵算法

    let _canvas = document.createElement("canvas");
    _canvas.width = this.box.width;
    _canvas.height = this.box.height;
    let _ctx = _canvas.getContext("2d")!;

    _ctx.textBaseline = "top";
    _ctx.fillStyle = this.dotColor;
    _ctx.font = this.box.Y + "px " + this.aText?.fontName ?? "宋体";
    _ctx.fillText(this.aText!.text, 0, 0);
    var imageData = _ctx.getImageData(0, 0, this.box.width, this.box.height);

    for (let x = 0; x < imageData.width; x += this.gridWidth) {
      for (let y = 0; y < imageData.height; y += this.gridWidth) {
        // 一个方格（gridWidth)为单位处理成一个led的像素点灯
        let threadhold = 0,
          z = 0;

        while (z < Math.pow(this.gridWidth, 2)) {
          let i = z % this.gridWidth;
          let j = Math.floor(z / this.gridWidth);
          let baseIndex = (x + i + (y + j) * imageData.width) * 4;
          // 25 91 255
          if (
            imageData.data[baseIndex] == 25 &&
            imageData.data[baseIndex + 1] == 91 &&
            imageData.data[baseIndex + 2] == 255
          ) {
            threadhold++;
            if (threadhold > Math.pow(this.gridWidth, 2) / 3) {
              // console.log(`这里应该着色${x},${y}`);
              this._dots.push(new Position(x, y, this.gridWidth, Math.floor));
              break;
            }
          }
          z++;
        }
      }
    }
  }

  get text(): ArtisicFont | undefined {
    return this.aText;
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

        let index = this._dots.findIndex((d) => d.equal(offsetPos));
        if (index >= 0) {
          this._dots.splice(index, 1);
        } else {
          this._dots.push(offsetPos);
        }
        // console.table(this.dots);
        break;
      default:
        throw new Error("unimplement code .");
    }
  }

  /**
   * todo: 改写draw
   * 除了点阵类型的对象输出私有_dots外，文字、图片，都需要重新
   * 计算的出每个像素点是否点亮，并以Positon的格式输出
   * 显示屏获得点阵数据直接绘制即可
   */
  get dots(): Array<Position> {
    return this._dots;
  }

  /**
   * 将点阵的内容转换程序
   */
  get PROGRAM(): string {
    return "";
  }
}
