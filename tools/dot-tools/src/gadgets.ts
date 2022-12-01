/**
 * 支持类
 * @author Django 2022-11-29
 */

/**
 * 坐标
 */
export class Position {
  private _x: number;
  private _y: number;
  // private gridWidth: number;
  constructor(
    X: number = 0,
    Y: number = 0,
    private readonly gridWidth: number = 1,
    private readonly pInt: Function = Math.round
  ) {
    this._x = X;
    this._y = Y;
  }

  get X(): number {
    return this.pInt(this._x / this.gridWidth) * this.gridWidth;
  }

  get Y() {
    return this.pInt(this._y / this.gridWidth) * this.gridWidth;
  }

  public equal(pos: Position) {
    return this.X === pos.X && this.Y === pos.Y;
  }
}

/**
 * 盒子对象（只有长宽）
 */
export class Box extends Position {
  constructor(width: number = 0, height: number = 0, gridWidth: number = 1) {
    super(...arguments);
  }

  get width(): number {
    return this.X;
  }

  get height(): number {
    return this.Y;
  }
}
