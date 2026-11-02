<h1 align="center">AlgoVis</h1>

<p>
AlgoVis is an interactive platform that aids in visualizing algorithms, reinforces the workings of algorithms, analyzes and compares different processes and factors such as time complexities, efficiency and makes data structures and algorithms less daunting
</p>
<p>
<h2> Features </h2> 
 <ul>
  <li> Speed adjustment </li>
  <li> Back and forth visualization </li>
  <li> Pause and Play </li>
  <li> Psuedocode highlight </li>
  <li> Code of the algorithm in C++, Java, and Python </li>
  <li> Custom and randomized input </li> 
 </ul>
</p>
<p>
  <h1> Screenshots </h1>
  <h2> Loading screen </h2>
  <img src="https://github.com/VaibhaveS/AlgoVis/blob/main/www/1.PNG">
  <h2> Bubble sort visualization in action </h2>
  <img src="https://github.com/VaibhaveS/AlgoVis/blob/main/www/2.PNG">
  <h2> Algorithm code in popular high level languages </h2>
  <img src="https://github.com/VaibhaveS/AlgoVis/blob/main/www/3.PNG">
  <h2> Linear search visualization in action </h2>
  <img src="https://github.com/VaibhaveS/AlgoVis/blob/main/www/4.PNG">
  <h2> Graph algorithms </h2>
  <img src="https://github.com/VaibhaveS/AlgoVis/blob/main/www/5.PNG">
  <h2> Kruskal's visualization in action </h2>
  <img src="https://github.com/VaibhaveS/AlgoVis/blob/main/www/6.PNG">
</p>

## Running the C++-connected data structure visualizers

This repository includes a persistent C++ data-structure backend connected to the browser through a small Node.js HTTP bridge.

1. Install Node.js 18+ and a C++17 compiler (`g++`).
2. Run `START_BACKEND_WINDOWS.bat` on Windows, or `node server.js` from this folder.
3. Open **http://localhost:3000** in your browser and keep the terminal running.

Do not use VS Code Live Server for these integrated modules; `server.js` serves the pages and forwards operations to the C++ process. See `INTEGRATION_README.md` for details.
