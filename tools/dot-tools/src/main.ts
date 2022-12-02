import "./style.css";
import { setupCanvas } from "./canvas";
// import { setupCounter } from "./counter";

document.querySelector<HTMLDivElement>("#app")!.innerHTML = `
  <div>
    <!-- 画布 -->
    <canvas id="myDots"></canvas>
  </div>
  <div id="control_panel">
    <!-- 操作界面 -->
  </div>
`;

// setupCounter(document.querySelector<HTMLButtonElement>("#counter")!);
setupCanvas(
  document.querySelector<HTMLCanvasElement>("#myDots")!,
  document.querySelector<HTMLElement>("#control_panel")!
);
