// UI layer only: hashing, chaining and key/value state are executed by C++.
let table = [],
  capacity = 8,
  entryCount = 0;
async function backend(op, value = "", value2 = "") {
  const r = await fetch("/api/operation", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ module: "hashmap", op, value, value2 }),
  });
  return r.json();
}
function showMessage(t, type) {
  const e = document.getElementById("message");
  e.textContent = t;
  e.className = "message " + (type || "");
}
function updateStats() {
  document.getElementById("statSize").textContent = entryCount;
  document.getElementById("statCapacity").textContent = capacity;
  document.getElementById("statLoadFactor").textContent = (
    entryCount / capacity
  ).toFixed(2);
}
function render() {
  const body = document.getElementById("hashBody");
  body.innerHTML = "";
  table.forEach((bucket, i) => {
    const tr = document.createElement("tr"),
      a = document.createElement("td"),
      b = document.createElement("td"),
      c = document.createElement("td");
    a.textContent = i;
    if (!bucket.length) {
      b.textContent = "(empty)";
      b.style.color = "#777";
    } else {
      bucket.forEach((e, j) => {
        if (j) b.append(" → ");
        const strong = document.createElement("b");
        strong.textContent = e.key;
        b.append(strong, document.createTextNode(" : " + e.value));
      });
    }
    c.textContent = bucket.length;
    tr.append(a, b, c);
    body.appendChild(tr);
  });
  updateStats();
}
async function sync(op, value = "", value2 = "") {
  try {
    const r = await backend(op, value, value2);
    table = r.buckets || [];
    entryCount = r.size || 0;
    capacity = r.capacity || 8;
    showMessage(r.message, r.ok ? "success" : "error");
    render();
    return r;
  } catch (e) {
    showMessage("Cannot reach C++ backend. Start server.js first.", "error");
  }
}
async function insert() {
  const k = document.getElementById("keyInput").value.trim(),
    v = document.getElementById("valInput").value.trim() || "0";
  if (!k) return showMessage("Please enter a key", "error");
  await sync("insert", k, v);
}
async function search() {
  const k = document.getElementById("keyInput").value.trim();
  if (!k) return showMessage("Enter key to search", "error");
  await sync("search", k);
}
async function removeKey() {
  const k = document.getElementById("keyInput").value.trim();
  if (!k) return showMessage("Enter key to delete", "error");
  await sync("remove", k);
}
async function clearMap() {
  await sync("clear");
}
async function loadExample() {
  await sync("loadExample");
}
(async () => {
  await sync("state");
})();
