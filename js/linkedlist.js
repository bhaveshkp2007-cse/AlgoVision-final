// UI layer only: all Linked List nodes and pointer operations are executed in C++.
let list = [];
async function backend(op, value = "", position = 0) {
  const r = await fetch("/api/operation", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ module: "linkedlist", op, value, position }),
  });
  return r.json();
}
function showMessage(text, type) {
  const el = document.getElementById("message");
  el.textContent = text;
  el.className = "message " + (type || "");
}
function updateInfo() {
  document.getElementById("sizeVal").textContent = list.length;
  document.getElementById("headVal").textContent = list.length
    ? list[0]
    : "NULL";
}
function render() {
  const box = document.getElementById("listDisplay");
  box.innerHTML = "";
  if (!list.length) {
    box.innerHTML =
      "<div style='text-align:center;color:#777;padding:20px;'>List is Empty (HEAD = NULL)</div>";
    return;
  }
  const container = document.createElement("div");
  container.className = "ll-container";
  const label = document.createElement("div");
  label.style.fontSize = "12px";
  label.innerHTML = "<b>HEAD</b> &rarr;";
  label.style.marginRight = "4px";
  container.appendChild(label);
  list.forEach((v, i) => {
    const node = document.createElement("div");
    node.className = "ll-node";
    const data = document.createElement("div");
    data.className = "ll-data";
    data.textContent = v;
    const next = document.createElement("div");
    next.className = "ll-next";
    next.textContent = i === list.length - 1 ? "NULL" : "next";
    node.append(data, next);
    container.appendChild(node);
    if (i < list.length - 1) {
      const arrow = document.createElement("div");
      arrow.className = "ll-arrow";
      arrow.textContent = "→";
      container.appendChild(arrow);
    }
  });
  box.appendChild(container);
}
async function sync(op, value = "", position = 0) {
  try {
    const r = await backend(op, value, position);
    list = r.items || [];
    showMessage(r.message, r.ok ? "success" : "error");
    updateInfo();
    render();
    return r;
  } catch (e) {
    showMessage("Cannot reach C++ backend. Start server.js first.", "error");
  }
}
function readValue() {
  return document.getElementById("llInput").value.trim();
}
async function insertBeginning() {
  const v = readValue();
  if (!v) return showMessage("Enter a value", "error");
  const r = await sync("insertBeginning", v);
  if (r && r.ok) document.getElementById("llInput").value = "";
}
async function insertEnd() {
  const v = readValue();
  if (!v) return showMessage("Enter a value", "error");
  const r = await sync("insertEnd", v);
  if (r && r.ok) document.getElementById("llInput").value = "";
}
async function insertAt() {
  const v = readValue(),
    p = parseInt(document.getElementById("llPosInput").value, 10);
  if (!v) return showMessage("Enter a value", "error");
  if (Number.isNaN(p)) return showMessage("Enter a valid position", "error");
  const r = await sync("insertAt", v, p);
  if (r && r.ok) document.getElementById("llInput").value = "";
}
async function deleteBeginning() {
  await sync("deleteBeginning");
}
async function deleteEnd() {
  await sync("deleteEnd");
}
async function deleteAt() {
  const p = parseInt(document.getElementById("llPosInput").value, 10);
  if (Number.isNaN(p)) return showMessage("Enter a valid position", "error");
  await sync("deleteAt", "", p);
}
async function search() {
  const v = document.getElementById("searchInput").value.trim();
  if (!v) return showMessage("Enter value to search", "error");
  await sync("search", v);
}
async function clearList() {
  await sync("clear");
}
(async () => {
  await sync("state");
})();
