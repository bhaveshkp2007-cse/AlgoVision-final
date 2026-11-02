// UI layer only: all Stack state and operations are executed by the C++ backend.
let stack = [],
  capacity = 8;
async function backend(op, value = "") {
  const r = await fetch("/api/operation", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ module: "stack", op, value }),
  });
  return r.json();
}
function showMessage(text, type) {
  const el = document.getElementById("message");
  el.textContent = text;
  el.className = "message " + (type || "");
}
function updateInfo() {
  const top = stack.length ? stack[stack.length - 1] : "Empty";
  document.getElementById("topVal").textContent = top;
  document.getElementById("sizeVal").textContent =
    stack.length + " / " + capacity;
  document.getElementById("isEmptyVal").textContent =
    stack.length === 0 ? "Yes" : "No";
  document.getElementById("isFullVal").textContent =
    stack.length >= capacity ? "Yes" : "No";
}
function render() {
  const box = document.getElementById("stackDisplay");
  box.innerHTML = "";
  if (!stack.length) {
    box.innerHTML =
      "<div style='text-align:center;color:#777;padding:30px;'>Stack is Empty (Top = -1)</div>";
    return;
  }
  const container = document.createElement("div");
  container.className = "stack-container";
  const stackBox = document.createElement("div");
  stackBox.className = "stack-box";
  stack.forEach((v, i) => {
    const item = document.createElement("div");
    item.className = "stack-item" + (i === stack.length - 1 ? " top" : "");
    item.textContent = v;
    stackBox.appendChild(item);
  });
  container.appendChild(stackBox);
  const labels = document.createElement("div");
  labels.className = "stack-labels";
  labels.innerHTML =
    "<div><b>TOP</b> = index " +
    (stack.length - 1) +
    "</div><div>Size: " +
    stack.length +
    "</div><div>Capacity: " +
    capacity +
    "</div>";
  container.appendChild(labels);
  box.appendChild(container);
}
async function sync(op, value = "") {
  try {
    const r = await backend(op, value);
    stack = r.items || [];
    capacity = r.capacity || 8;
    showMessage(r.message, r.ok ? "success" : "error");
    updateInfo();
    render();
    return r;
  } catch (e) {
    showMessage("Cannot reach C++ backend. Start server.js first.", "error");
  }
}
async function push() {
  const input = document.getElementById("stackInput"),
    v = input.value.trim();
  if (!v) {
    showMessage("Please enter a value to push", "error");
    return;
  }
  const r = await sync("push", v);
  if (r && r.ok) input.value = "";
}
async function pop() {
  await sync("pop");
}
async function peek() {
  await sync("peek");
}
async function clearStack() {
  await sync("clear");
}
(async () => {
  const r = await sync("state");
  if (!r) showMessage("C++ backend unavailable", "error");
})();
