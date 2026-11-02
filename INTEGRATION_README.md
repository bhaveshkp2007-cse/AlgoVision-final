# AlgoVis — C++ backend integration

The browser no longer maintains a second copy of the data structures. All Stack, Queue, Linked List and HashMap operations are sent to the persistent C++ process in `backend/visualizers.cpp`. JavaScript is the presentation layer: it sends user actions to the local Node bridge and renders the JSON state returned by C++.

## Requirements
- Node.js 18+
- g++ with C++17 support

## Run on Windows
1. Extract the whole project folder.
2. Double-click `START_BACKEND_WINDOWS.bat`, or run `node server.js` in the project folder.
3. Open **http://localhost:3000** in your browser.
4. Keep the terminal window open while using AlgoVis.

The first launch compiles `backend/visualizers.cpp` into `backend/visualizers.exe`. Subsequent launches reuse the executable unless it is deleted.

## Run on macOS/Linux
Run `chmod +x START_BACKEND_MAC_LINUX.sh` once, then `./START_BACKEND_MAC_LINUX.sh`.

## Architecture
HTML/CSS -> JavaScript `fetch()` -> Node HTTP bridge -> persistent C++ process -> JSON response -> JavaScript rendering.

The C++ process owns the live data-structure state. Stack and Queue store strings in vectors; Linked List uses dynamically allocated `Node` objects and pointers; HashMap uses separate chaining with key-value pairs.

## Important
Do not open the HTML files directly with `file://` or use VS Code Live Server for this integrated build. Open `http://localhost:3000` after starting the backend. The old C++ demo `main()` has been replaced by a request loop so the C++ process remains alive and handles operations from the browser.
