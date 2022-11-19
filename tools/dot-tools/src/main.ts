import "./style.css";
import { setupCanvas } from "./canvas";
// import { setupCounter } from "./counter";

document.querySelector<HTMLDivElement>("#app")!.innerHTML = `
  <div>
    <canvas id="myDots"></canvas>
  </div>
`;

// setupCounter(document.querySelector<HTMLButtonElement>("#counter")!);
setupCanvas(document.querySelector<HTMLCanvasElement>("#myDots")!);
