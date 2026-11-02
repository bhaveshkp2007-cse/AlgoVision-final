// UI layer only: queue state and operations are executed by C++.
let queue = [],
  capacity = 8;
async function backend(op, value = "") {
  const r = await fetch("/api/operation", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ module: "queue", op, value }),
  });
  return r.json();
}
function showMessage(t, type) {
  const e = document.getElementById("message");
  e.textContent = t;
  e.className = "message " + (type || "");
}
function updateInfo() {
  document.getElementById("frontVal").textContent = queue.length
    ? queue[0]
    : "Empty";
  document.getElementById("sizeVal").textContent =
    queue.length + " / " + capacity;
  document.getElementById("isEmptyVal").textContent = queue.length
    ? "No"
    : "Yes";
  document.getElementById("isFullVal").textContent =
    queue.length >= capacity ? "Yes" : "No";
}
function render() {
  const box = document.getElementById("queueDisplay");
  box.innerHTML = "";
  if (!queue.length) {
    box.innerHTML =
      "<div style='text-align:center;color:#777;padding:20px;'>Queue is Empty (Front = -1, Rear = -1)</div>";
    return;
  }
  const c = document.createElement("div");
  c.className = "queue-container";
  const f = document.createElement("div");
  f.style.fontSize = "12px";
  f.innerHTML = "<b>FRONT</b> &rarr;";
  c.appendChild(f);
  queue.forEach((v, i) => {
    const x = document.createElement("div");
    x.className = "queue-item" + (i === 0 ? " front" : "");
    x.textContent = v;
    c.appendChild(x);
    if (i < queue.length - 1) {
      const a = document.createElement("div");
      a.className = "queue-arrow";
      a.textContent = "→";
      c.appendChild(a);
    }
  });
  const rear = document.createElement("div");
  rear.style.fontSize = "12px";
  rear.innerHTML = "&rarr; <b>REAR</b>";
  c.appendChild(rear);
  box.appendChild(c);
}
async function sync(op, value = "") {
  try {
    const r = await backend(op, value);
    queue = r.items || [];
    capacity = r.capacity || 8;
    showMessage(r.message, r.ok ? "success" : "error");
    updateInfo();
    render();
    return r;
  } catch (e) {
    showMessage("Cannot reach C++ backend. Start server.js first.", "error");
  }
}
async function enqueue() {
  const i = document.getElementById("queueInput"),
    v = i.value.trim();
  if (!v) return showMessage("Please enter a value to enqueue", "error");
  const r = await sync("enqueue", v);
  if (r && r.ok) i.value = "";
}
async function dequeue() {
  await sync("dequeue");
}
async function front() {
  await sync("front");
}
async function clearQueue() {
  await sync("clear");
}
(async () => {
  await sync("state");
})();
