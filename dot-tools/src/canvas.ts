const DISPLAY_WIDTH = 128,
  DISPLAY_HEIGHT = 64;
// 双色屏幕
const DUAL_COLOR = true;
// 画布的宽度与高度
let WIDTH: number, HEIGHT: number, GRID_WIDTH: number;
// 网格线颜色
const LINE_COLOR: string = "rgb(128,128,128)";
const LINE_COLOR_2: string = "rgb(192,192,192)";
// 网格与灯的宽度比例，1线5灯
const LINE_LIGHT_RATIO = 1 / 6;

export function setupCanvas(cvs: HTMLCanvasElement) {
  // console.log(document.body.clientWidth);
  WIDTH = cvs.parentElement!.clientWidth; // document.body.clientWidth;
  HEIGHT = WIDTH / (DISPLAY_WIDTH / DISPLAY_HEIGHT);
  GRID_WIDTH = WIDTH / DISPLAY_WIDTH;

  cvs.width = WIDTH;
  cvs.height = HEIGHT;
  // 计算框的宽度
  let lineWidth = (WIDTH / DISPLAY_WIDTH) * LINE_LIGHT_RATIO;
  const ctx = cvs.getContext("2d")!;

  ctx.fillStyle = "black";
  ctx.fillRect(0, 0, WIDTH, HEIGHT);
  ctx.lineWidth = lineWidth;

  ctx.beginPath();
  ctx.strokeStyle = LINE_COLOR;
  for (let i = 0; i < DISPLAY_WIDTH; i++) {
    ctx.moveTo((1 + i) * GRID_WIDTH, 0);
    ctx.lineTo((1 + i) * GRID_WIDTH, HEIGHT);
  }
  ctx.stroke();

  for (let i = 0; i < DISPLAY_HEIGHT; i++) {
    ctx.beginPath();
    ctx.strokeStyle = DUAL_COLOR && i === 15 ? LINE_COLOR_2 : LINE_COLOR;
    ctx.moveTo(0, (i + 1) * GRID_WIDTH);
    ctx.lineTo(WIDTH, (i + 1) * GRID_WIDTH);
    ctx.stroke();
  }

  cvs.onclick = function (e) {
    // let p = new Position(e.offsetX / GRID_WIDTH, e.offsetY / GRID_WIDTH);
    // console.log(`Click at (${p.x},${p.y})`);
  };

  // shange
}
