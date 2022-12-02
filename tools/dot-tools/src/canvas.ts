import { Display } from "./display";

const LINE_LIGHT_RATIO = 1 / 6;

export function setupCanvas(cvs: HTMLCanvasElement, controlPanel: HTMLElement) {
  cvs.width = cvs.parentElement!.clientWidth;
  cvs.height = cvs.width / 2;

  const display = new Display(cvs, 128, 64, true, "#195BFF", controlPanel);
}
