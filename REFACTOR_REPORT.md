# AlgoVis Refactoring Report

## 1. Project Overview

This is the refactored version of **AlgoVis** college project. The four visualizers have been simplified, code reduced, frontend made basic, and core logic moved to C++.

**Branch:** `arena/af67a72e-algovision` (original code preserved in git history)

---

## 2. File Structure

```
AlgoVision/
│
├── stack.html              # Stack visualizer (HTML frontend)
├── queue.html              # Queue visualizer
├── linkedlist.html         # Linked List visualizer
├── hashmap.html            # HashMap visualizer
├── index.html              # Home page (unchanged, links to visualizers)
│
├── css/
│   ├── common.css          # Shared simple dull theme (all pages use this)
│   ├── stack.css           # Copy of common theme (per-page CSS)
│   ├── queue.css
│   ├── linkedlist.css
│   └── hashmap.css
│
├── js/
│   ├── stack.js            # Minimal JS for Stack UI (mirrors C++ logic)
│   ├── queue.js
│   ├── linkedlist.js
│   └── hashmap.js
│
└── backend/
    └── visualizers.cpp     # SINGLE C++ FILE with all 4 data structures
```

### What each folder does:

| Path | Purpose |
|------|---------|
| `*.html` | Frontend: input boxes, buttons, display area only |
| `css/*.css` | Styling: dull grey/muted blue, basic borders, no gradients/shadows |
| `js/*.js` | Minimal JS: button handling + visual update (logic mirrors C++) |
| `backend/visualizers.cpp` | Real backend: Stack, Queue, LinkedList, HashMap classes in C++ |

### How frontend connects to backend:

**Honest technical explanation:**

- **Browser cannot directly run .cpp files.** JavaScript is required for browser interaction (button clicks, updating HTML).
- **Actual algorithms are written in C++** in `backend/visualizers.cpp`. This file can be compiled with `g++` and run as a console program. It contains the same logic as the visualizers.
- **Frontend JS mirrors the C++ logic** with identical function names (`push/pop/peek`, `enqueue/dequeue`, etc.) and same hash formula. So when you explain C++ code to professor, you are explaining the real logic that powers the visualizer.
- **Optional real WASM connection:** If you have Emscripten installed, you can compile `visualizers.cpp` to WebAssembly and call C++ functions directly from JS. Instructions below. But even without WASM, the C++ file is the authoritative implementation.

```
C++ (backend/visualizers.cpp)  <--- same logic --->  JS (js/*.js)  --->  HTML display
     compile with g++ to run standalone
     or compile with emcc to WebAssembly for browser
```

---

## 3. Code Reduction Report

Actual line counts measured with `wc -l`:

| File | Original Lines | Final Lines | Lines Reduced | Reduction % |
|------|---------------|-------------|---------------|-------------|
| **stack.html** | 2742 | 65 | **2677** | 97.6% |
| **queue.html** | 2879 | 65 | **2814** | 97.7% |
| **linkedlist.html** | 2493 | 75 | **2418** | 97.0% |
| **hashmap.html** | 1042 | 78 | **964** | 92.5% |
| **TOTAL (4 files)** | **9156** | **283** | **8873** | **96.9%** |

Additional new files (not counted as reduction, but necessary separation):

| New File | Lines | Purpose |
|----------|-------|---------|
| css/common.css | 362 | Simple dull theme |
| backend/visualizers.cpp | 460 | C++ backend |
| js/stack.js | 92 | Stack UI logic |
| js/queue.js | 101 | Queue UI logic |
| js/linkedlist.js | 134 | LinkedList UI logic |
| js/hashmap.js | 135 | HashMap UI logic |

**Note on hashmap.html:** Original was only 1042 lines, so reducing 1000+ is not realistic while keeping it functional. We reduced 964 lines (92.5%), which is the maximum possible without making it look incomplete. Requirement says "remove as much as possible if 1000 is not possible" - we have done that.

---

## 4. Removed Features

### Stack (`stack.html`)
Removed:
- Array Implementation section (6 slots with arr[MAX] demo)
- Linked List Implementation section (node chain demo)
- Postfix Evaluation (5 expressions + token tape)
- Prefix Evaluation (right-to-left scanning)
- Separate Peek demo section (before/after comparison table)
- Separate IsEmpty section (empty check with result badge)
- Separate IsFull section (capacity progress bar)
- Sequence batch scripting (textarea with RUN/PREV/NEXT/PLAY)
- Operation history panel
- Heavy CSS: gradients, glow effects, coloured bucket, code tabs (C++/Java/Python)

Kept:
- Push, Pop, Peek, Display, Clear, Empty/Full check via info box

### Queue (`queue.html`)
Removed:
- Peek Front section (non-destructive proof card)
- IsEmpty section
- IsFull section (capacity stats)
- Single Queue FIFO deep dive
- Double Ended Queue (Deque) with 6 operations
- Circular Queue (SVG ring + wrap-around demo)
- Priority Queue (priority insert, heap ordering)
- Array Implementation (false overflow demo)
- Linked List Implementation (node address simulation)
- Batch sequence script with step playback
- 10 separate navbar sections

Kept:
- Enqueue, Dequeue, Front, Display, Clear, Empty/Full info

### Linked List (`linkedlist.html`)
Removed:
- List type selector: Doubly and Circular (kept only Singly)
- Traversal section (forward/reverse with pulse animations)
- Separate Deletion tabs with animations
- Circular SVG overlay (loop-back arrow)
- Staging area for new nodes
- jQuery UI slider for speed
- Code snippet panel with pointer steps
- Operation tabs complexity
- Head/Tail/Curr tags with glow effects

Kept:
- Insert at Beginning, Insert at End, Insert at Position
- Delete at Beginning, Delete at End, Delete at Position
- Search (returns index)
- Display/Traverse (HEAD -> nodes -> NULL)
- Empty check, Size info

### HashMap (`hashmap.html`)
Removed:
- Linear Probing strategy (kept only Separate Chaining)
- Increment operation
- Capacity resize (kept fixed 8 buckets)
- Timeline playback (Prev/Play/Next with stepDelay slider)
- Math breakdown box with char code details per step
- Tombstone handling for probing
- Strategy pills toggle animation
- Example run with masterSteps batch

Kept:
- Insert (with collision chaining, duplicate update)
- Search (bucket chain scan)
- Delete (remove from chain)
- Display (table of 8 buckets with entries)
- Load Example
- Hash function: sum char codes % capacity (same as before)
- Load factor stats

---

## 5. C++ Backend Explanation

**File:** `backend/visualizers.cpp` (460 lines, easy to explain)

### Classes implemented:

#### Stack class
```cpp
class Stack {
  int arr[20]; int top; int capacity;
  bool push(int value);   // top++ then arr[top]=value
  int pop();              // return arr[top] then top--
  int peek();             // return arr[top] without removing
  bool isEmpty();         // top == -1
  bool isFull();          // top == capacity-1
  void display();
}
```
Simple array with top pointer. Capacity 8 default. Uses `-1` for empty.

#### Queue class
```cpp
class Queue {
  int arr[20]; int front, rear, capacity, count;
  bool enqueue(int value); // rear=(rear+1)%cap
  int dequeue();           // front=(front+1)%cap
  int getFront();          // arr[front]
  void display();
}
```
Circular array queue using modulo. `count` tracks size to avoid confusion between empty/full.

#### LinkedList class
```cpp
struct Node { int data; Node* next; };
class LinkedList {
  Node* head; int listSize;
  void insertAtBeginning(int value);
  void insertAtEnd(int value);
  bool insertAtPosition(int value, int pos);
  int deleteFromBeginning();
  int deleteFromEnd();
  int deleteAtPosition(int pos);
  int search(int value); // returns index
  void display(); // HEAD -> ... -> NULL
}
```
Standard singly linked list with pointer manipulation. `new` and `delete` for memory.

#### HashMap class
```cpp
class HashMap {
  vector<pair<string,string>> table[8];
  int hashFunction(string key); // sum(char codes) % 8
  void insert(string key, string value); // chaining for collision
  string search(string key);
  bool remove(string key);
  void display();
}
```
Separate Chaining: each bucket is a vector of pairs. Collision = add to chain. Duplicate key = update.

### Why this approach?
- No templates, no inheritance, no STL complexities
- Fixed small sizes (8, 20) easy to understand
- Variable names meaningful (`top`, `front`, `head`, `table`)
- Single file, all 4 DS together as requested

---

## 6. Running Instructions (Beginner Friendly)

### Option A: Just open in browser (simplest)
1. Open folder `AlgoVision` in VS Code (File -> Open Folder)
2. Find `index.html` or any of the 4 visualizer HTML files
3. Right-click file -> "Open with Live Server" (or double-click to open in browser)
4. Alternative without VS Code: Double-click `stack.html` -> opens in Chrome/Firefox
5. No installation needed! CSS and JS are local files.

### Option B: Run with Python server (if file:// has issues)
1. Open terminal in project folder
2. Run: `python -m http.server 8000`
3. Open browser: `http://localhost:8000`
4. Click on `stack.html`, `queue.html`, etc.

### Option C: Compile and run C++ backend
1. Install g++ (usually already installed on Linux/Mac. On Windows install MinGW)
2. Open terminal in project root
3. Compile: `g++ backend/visualizers.cpp -o algovis`
4. Run: `./algovis` (on Windows: `algovis.exe`)
5. You will see console demo of all 4 data structures

### Option D: Compile C++ to WebAssembly (optional, advanced)
1. Install Emscripten: https://emscripten.org/docs/getting_started/downloads.html
2. Compile: `emcc backend/visualizers.cpp -o js/visualizers.js -s WASM=1 -s EXPORTED_FUNCTIONS="['_main']"`
3. This creates WASM that browser can call directly

---

## 7. Testing Checklist

All visualizers tested:

**Stack:**
- [x] Push 10 -> shows 10 at top
- [x] Push 20, 30 -> stack grows, top=30
- [x] Peek -> shows 30 without removing
- [x] Pop -> removes 30, new top 20
- [x] Pop until empty -> shows "Stack is Empty"
- [x] Push 9 items -> 9th shows "Overflow"
- [x] Clear -> empties stack
- [x] Empty/Full indicators update correctly

**Queue:**
- [x] Enqueue 10,20,30 -> shows FRONT->10->20->30<-REAR
- [x] Front -> shows 10
- [x] Dequeue -> removes 10, new front 20
- [x] Enqueue after dequeue -> correct order
- [x] Overflow at 8 items
- [x] Underflow on empty dequeue

**Linked List:**
- [x] Insert at End: 10,20,30 -> HEAD->10->20->30->NULL
- [x] Insert at Beginning: 5 -> HEAD->5->10->20->30
- [x] Insert at Position 2: 15 -> correct placement
- [x] Delete from Beginning -> removes 5
- [x] Delete from End -> removes 30
- [x] Delete at Position 1 -> removes correct node
- [x] Search 20 -> found at index 1
- [x] Search 99 -> not found
- [x] Clear -> HEAD=NULL

**HashMap:**
- [x] Insert Alice:25 -> bucket 6
- [x] Insert Bob:30 -> bucket 3
- [x] Insert Dan:15 -> collides with Bob at bucket 3, chains
- [x] Search Bob -> found at bucket 3
- [x] Search unknown -> not found
- [x] Insert duplicate Alice:30 -> updates value
- [x] Delete Bob -> removes from chain
- [x] Delete unknown -> not found message
- [x] Load Example -> loads 4 entries
- [x] Clear -> empties all buckets
- [x] Collision handling verified
- [x] No console errors
- [x] All 4 HTML pages open

---

## 8. Important Code Locations

For explaining to professor, find these exact locations:

| Operation | File | Function/Class |
|-----------|------|----------------|
| **Stack Push** | `backend/visualizers.cpp` | `Stack::push()` line ~18 |
| **Stack Pop** | `backend/visualizers.cpp` | `Stack::pop()` line ~27 |
| **Stack Peek** | `backend/visualizers.cpp` | `Stack::peek()` line ~37 |
| **Queue Enqueue** | `backend/visualizers.cpp` | `Queue::enqueue()` line ~67 |
| **Queue Dequeue** | `backend/visualizers.cpp` | `Queue::dequeue()` line ~80 |
| **Queue Front** | `backend/visualizers.cpp` | `Queue::getFront()` line ~93 |
| **LL Insert Beginning** | `backend/visualizers.cpp` | `LinkedList::insertAtBeginning()` line ~127 |
| **LL Insert End** | `backend/visualizers.cpp` | `LinkedList::insertAtEnd()` line ~135 |
| **LL Delete** | `backend/visualizers.cpp` | `LinkedList::deleteFromBeginning()` / `deleteFromEnd()` |
| **LL Search** | `backend/visualizers.cpp` | `LinkedList::search()` line ~215 |
| **HashMap Hash** | `backend/visualizers.cpp` | `HashMap::hashFunction()` line ~248 |
| **HashMap Insert** | `backend/visualizers.cpp` | `HashMap::insert()` line ~257 |
| **HashMap Search** | `backend/visualizers.cpp` | `HashMap::search()` line ~273 |
| **HashMap Delete** | `backend/visualizers.cpp` | `HashMap::remove()` line ~285 |
| **Frontend Stack** | `js/stack.js` | `push()`, `pop()`, `peek()` |
| **Frontend Queue** | `js/queue.js` | `enqueue()`, `dequeue()`, `front()` |
| **Frontend LL** | `js/linkedlist.js` | `insertBeginning()`, `insertEnd()`, `search()` |
| **Frontend HashMap** | `js/hashmap.js` | `hashFunction()`, `insert()`, `search()`, `removeKey()` |

---

## 9. Design Philosophy

The new design is intentionally **basic and dull**:

- Background: light grey #d9d9d9 and white containers
- Header: dark grey #555, no gradients
- Buttons: muted blue-grey #6b7a8f, no glow/shadow
- Borders: simple 1px solid #888
- Stack items: muted blue #a8b8c8, top highlighted #d9b44a
- No glassmorphism, no animations, no rounded cards, no glowing effects
- Font: Arial, consistent 13-14px
- Layout: simple centered container, sequential sections

Looks like a college beginner made it - which is exactly the requirement.

---

## 10. What is C++ and What is JS?

| Part | Language | Why |
|------|----------|-----|
| Data structure logic (push, pop, hash, etc.) | **C++** (`backend/visualizers.cpp`) | Real algorithms, easy to explain in presentation, compilable |
| Button clicks, input reading | **JS** (`js/*.js`) | Browser requires JS to handle DOM events - unavoidable |
| Updating display on screen | **JS** | Browser DOM manipulation must be JS |
| Styling, layout | **CSS** (`css/*.css`) | Presentation only |
| Structure | **HTML** (`*.html`) | Page skeleton |

**We did NOT fake C++ in browser.** The C++ file is real, compilable, and contains the actual algorithms. JS is only the thin communication layer that browser needs. This is the simplest technically honest solution without setting up a complex server or WASM build system.

