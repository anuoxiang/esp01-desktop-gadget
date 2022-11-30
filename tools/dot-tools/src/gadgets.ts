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
  private grid_width: number;
  constructor(X: number = 0, Y: number = 0, gridWidth: number = 0) {
    this._x = X;
    this._y = Y;
    this.grid_width = gridWidth;
  }

  get X(): number {
    if (this.grid_width > 0) {
      return Math.round(this._x / this.grid_width) * this.grid_width;
    } else {
      return this._x;
    }
  }

  get Y() {
    if (this.grid_width > 0) {
      return Math.round(this._y / this.grid_width) * this.grid_width;
    } else {
      return this._y;
    }
  }
}

/**
 * 盒子对象（只有长宽）
 */
export class Box extends Position {
  constructor(width: number = 0, height: number = 0, gridWidth: number = 0) {
    super(...arguments);
  }

  get width(): number {
    return this.X;
  }

  get height(): number {
    return this.Y;
  }
}
