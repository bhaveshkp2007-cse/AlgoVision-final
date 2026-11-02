const http = require("http");
const fs = require("fs");
const path = require("path");
const { spawn, execFileSync } = require("child_process");
const readline = require("readline");
const ROOT = __dirname;
const isWin = process.platform === "win32";
const exe = path.join(
  ROOT,
  "backend",
  isWin ? "visualizers.exe" : "visualizers",
);
try {
  if (!fs.existsSync(exe))
    execFileSync(
      isWin ? "g++" : "g++",
      [
        path.join(ROOT, "backend", "visualizers.cpp"),
        "-std=c++17",
        "-O2",
        "-o",
        exe,
      ],
      { stdio: "inherit" },
    );
} catch (e) {
  console.error("Could not compile C++ backend. Install g++ and retry.");
  process.exit(1);
}
const backend = spawn(exe, [], { stdio: ["pipe", "pipe", "inherit"] });
const lines = readline.createInterface({ input: backend.stdout });
const waiting = [];
lines.on("line", (line) => {
  const item = waiting.shift();
  if (item) {
    try {
      item.resolve(JSON.parse(line));
    } catch (e) {
      item.reject(e);
    }
  }
});
backend.on("exit", (code) => {
  while (waiting.length)
    waiting.shift().reject(new Error("C++ backend exited: " + code));
});
function encode(s = "") {
  return encodeURIComponent(String(s));
}
function callCpp(data) {
  return new Promise((resolve, reject) => {
    waiting.push({ resolve, reject });
    const row =
      [
        data.module,
        data.op,
        data.value || "",
        data.value2 || "",
        data.position ?? "",
      ]
        .map(encode)
        .join("\t") + "\n";
    backend.stdin.write(row);
  });
}
const types = {
  ".html": "text/html; charset=utf-8",
  ".js": "text/javascript; charset=utf-8",
  ".css": "text/css; charset=utf-8",
  ".png": "image/png",
  ".jpg": "image/jpeg",
  ".gif": "image/gif",
  ".svg": "image/svg+xml",
  ".ico": "image/x-icon",
};
const server = http.createServer(async (req, res) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  res.setHeader("Access-Control-Allow-Headers", "Content-Type");
  if (req.method === "OPTIONS") {
    res.writeHead(204);
    return res.end();
  }
  if (req.url === "/api/health") {
    res.writeHead(200, { "Content-Type": "application/json" });
    return res.end(JSON.stringify({ status: "running", backend: "C++" }));
  }
  if (req.url === "/api/operation" && req.method === "POST") {
    let body = "";
    req.on("data", (c) => (body += c));
    req.on("end", async () => {
      try {
        const data = JSON.parse(body);
        const result = await callCpp(data);
        res.writeHead(200, { "Content-Type": "application/json" });
        res.end(JSON.stringify(result));
      } catch (e) {
        res.writeHead(500, { "Content-Type": "application/json" });
        res.end(JSON.stringify({ ok: false, message: e.message }));
      }
    });
    return;
  }
  const pathname = decodeURIComponent(
    new URL(req.url, "http://localhost").pathname,
  );
  const rel = pathname === "/" ? "index.html" : pathname.replace(/^\/+/, "");
  const file = path.resolve(ROOT, rel);
  if (!file.startsWith(ROOT + path.sep) && file !== ROOT) {
    res.writeHead(403);
    return res.end("Forbidden");
  }
  fs.readFile(file, (err, data) => {
    if (err) {
      res.writeHead(404);
      return res.end("Not found");
    }
    res.writeHead(200, {
      "Content-Type": types[path.extname(file)] || "application/octet-stream",
    });
    res.end(data);
  });
});
server.listen(3000, "127.0.0.1", () =>
  console.log("AlgoVis: http://localhost:3000 (C++ backend connected)"),
);
process.on("SIGINT", () => {
  backend.kill();
  server.close(() => process.exit(0));
});
