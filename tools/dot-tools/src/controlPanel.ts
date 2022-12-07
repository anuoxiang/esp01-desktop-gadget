import { ElementType } from "./dotsElement";

type TypeChangeFunction = (type: ElementType) => void;
type ContentChangeFunction = (content: string) => void;

/**
 * 生成控制面板表单
 * @param controlPanel 控制面板的根对象节点
 * @param onTypeChange 响应类型改变
 * @param onContentChange 响应文字内容改变
 */
export function generateControlPanelForm(
  controlPanel: HTMLElement,
  onTypeChange: TypeChangeFunction,
  onContentChange: ContentChangeFunction
) {
  let typeDes = document.createTextNode("元素类型：");
  let elementType = document.createElement<"select">("select");
  elementType.id = "element_type";
  elementType.disabled = true;
  let opt1 = document.createElement("option");
  opt1.value = "0";
  opt1.innerText = "文本";
  elementType.appendChild(opt1);

  let opt2 = document.createElement("option");
  opt2.value = "1";
  opt2.innerText = "图片";
  opt2.disabled = true; // 暂不支持
  elementType.appendChild(opt2);

  let opt3 = document.createElement("option");
  opt3.value = "2";
  opt3.innerText = "动画";
  opt3.disabled = true; //暂不支持s
  elementType.appendChild(opt3);

  let opt4 = document.createElement("option");
  opt4.value = "3";
  opt4.innerText = "点阵";
  elementType.appendChild(opt4);

  let inputDes = document.createTextNode("输入文字：");
  let elementContent = document.createElement<"input">("input");
  elementContent.id = "element_content";
  elementContent.disabled = true;
  elementContent.type = "text";

  elementType.onchange = () => {
    onTypeChange(parseInt(elementType.value));
    console.log("修改类型");
  };

  elementContent.onchange = () => {
    console.log("修改内容");
    onContentChange(elementContent.value);
  };

  controlPanel.appendChild(typeDes);
  controlPanel.appendChild(elementType);
  controlPanel.appendChild(inputDes);
  controlPanel.appendChild(elementContent);
}
