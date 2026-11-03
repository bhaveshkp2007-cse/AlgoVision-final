# AlgoVis C++ Backend (Modular Version)

## Architecture

- `visualizers.cpp`: program entry point and request dispatcher.
- `Stack.h/.cpp`: array-backed Stack ADT.
- `Queue.h/.cpp`: circular Queue ADT.
- `LinkedList.h/.cpp`: singly linked list ADT.
- `HashMap.h/.cpp`: hash table using separate chaining.
- `JsonUtils.h/.cpp`: JSON escaping and response helpers shared by modules.

The Node.js server (`server.js`) accepts browser requests at `POST /api/operation`.
It forwards each request to one persistent C++ process using one tab-separated,
URL-encoded input line. The C++ program returns exactly one JSON line per request.

**Important:** `visualizers.cpp` flushes standard output after every response. This is
required because Node.js reads the C++ process output through a pipe; without an
explicit flush, requests can remain pending even though the C++ operation completed.

## Build manually

From the project root:

```bash
g++ backend/visualizers.cpp backend/Stack.cpp backend/Queue.cpp backend/LinkedList.cpp backend/HashMap.cpp backend/JsonUtils.cpp -std=c++17 -O2 -o backend/visualizers
```

On Windows, the server compiles the same sources to `backend/visualizers.exe` when
that executable is missing.
