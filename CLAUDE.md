# Travel Plan Optimizer — CLAUDE.md

## Project Overview

Single-file C console application that helps users plan India-only travel itineraries.
The program has two phases:

1. **Itinerary builder** — collects destinations and activities into a tree structure
   (`TreeNode`) where each destination node holds a linked list of activity child nodes.
2. **Route optimizer** — accepts a separate list of destinations with distances and
   runs Dijkstra's algorithm to find the shortest path between a user-chosen source
   and destination.

**Source file:** `Travel-Plan-Optimizer-main/Travel Planner Optimiser.c`
**Language:** C (C99)
**Platform:** Windows (primary), cross-platform compatible
**Scope:** India-only destinations; 8 pre-defined suggestions in `suggestedList[]`

---

## Build Commands

### GCC / MinGW (preferred on Windows)
```
gcc "Travel Planner Optimiser.c" -o travel_planner -Wall -Wextra -std=c99
```

### Clang
```
clang "Travel Planner Optimiser.c" -o travel_planner -Wall -Wextra -std=c99
```

### MSVC (Developer Command Prompt)
```
cl "Travel Planner Optimiser.c" /Fe:travel_planner.exe /W4
```

### Run
```
./travel_planner        # GCC/Clang
travel_planner.exe      # Windows
```

---

## Code Style

Match the existing style exactly when editing or extending this file.

### Naming
| Construct | Convention | Example |
|---|---|---|
| Macros / constants | `UPPER_SNAKE_CASE` | `MAX_NAME_LENGTH`, `NUM_SUGGESTED` |
| Struct tags | `PascalCase` | `TreeNode`, `SuggestedDestination` |
| Functions | `camelCase` | `addNode()`, `calculateTotalCost()` |
| Local variables | `camelCase` | `remainingDays`, `numActivities` |
| Loop counters | single letter | `i`, `v`, `u` |

### Formatting
- **Indentation:** 4 spaces, no tabs
- **Braces:** opening brace on the same line as control structure or function signature
- **Line comments:** `//` above the block being described, not inline unless very short
- **Pointer declarations:** `Type* varName` (asterisk with type, not variable)
- All strings in `printf` use aligned `%-Ns` format specifiers for tabular output

### String safety
- Always use `strncpy` with `sizeof(dest) - 1`, then explicitly null-terminate:
  ```c
  strncpy(node->name, name, sizeof(node->name) - 1);
  node->name[sizeof(node->name) - 1] = '\0';
  ```
- Never use `strcpy` or unbounded `gets`.

### Input handling
- Use `scanf` for numeric input inside `do { } while` validation loops.
- Always call `getchar()` after `scanf` to consume the trailing newline before `fgets`.
- Use `fgets` + `strcspn(buf, "\n") = 0` for string input.

---

## Known Issues (from Code Review)

These are confirmed bugs. Fix them before adding new features.

### Critical
| # | Location | Issue |
|---|---|---|
| 1 | Lines 434–439 | **Memory leak:** only the first level of activity nodes is freed; `node->activities` children are never recursively freed. Replace the flat loop with a `freeTree(struct TreeNode*)` recursive function. |
| 2 | Line 401 | **Buffer overflow:** `numDestinationsInput++` has no bounds check against `MAX_DESTINATIONS (10)`. Guard with `if (numDestinationsInput >= MAX_DESTINATIONS) break;` before incrementing. |
| 3 | Lines 100–154 | **Dijkstra is logically broken:** uses `destinations[u].distance` (a single "distance to next" value) as the weight for *all* neighbours instead of an adjacency matrix. The algorithm cannot find correct shortest paths without a proper 2-D distance matrix. |
| 4 | Line 142 | **NULL dereference:** if all nodes are unreachable, `u` stays `-1` and `visited[-1] = 1` is undefined behaviour. |

### High
| # | Location | Issue |
|---|---|---|
| 5 | Line 61 | **Missing NULL check:** `malloc()` return value is used immediately without checking for `NULL`. |
| 6 | Lines 213, 227, 233, 257, 263, 290, 298, 359, 372, 394 | **Unchecked `scanf` returns:** non-numeric input causes `scanf` to fail silently, leaving variables uninitialised and validation loops infinite. Check the return value and flush the buffer on failure. |
| 7 | Line 145 | **Integer overflow:** `destinations[u].distance + dist[u]` overflows when `dist[u] == INT_MAX`. Guard with `if (dist[u] != INT_MAX && ...)`. |

### Medium
- Destination input logic is duplicated between the itinerary phase and route optimisation phase — extract to a shared function.
- `calculateTotalCost` and `calculateTotalDays` include the root node's values (always 0), which is harmless but semantically incorrect.
- O(n) linked-list insertion in `addNode` — consider maintaining a tail pointer.

---

## Coding Standards

These apply to all changes in this project.

1. **No dynamic allocation without a NULL check.** Every `malloc`/`calloc` call must be followed by `if (ptr == NULL) { /* handle */ }`.

2. **Every allocation must have a matching free.** Use a recursive `freeTree()` for tree nodes, not a flat loop. Prefer RAII-style patterns: allocate at the start of a scope, free at the end.

3. **Validate all user input.** `scanf` return values must be checked. On failure, flush stdin with `while (getchar() != '\n');` before re-prompting.

4. **Bounds-check all array writes.** Before writing to a fixed-size array (e.g., `destinations[]`), verify the index is within bounds.

5. **No magic numbers.** Use the existing `#define` constants (`MAX_NAME_LENGTH`, `MAX_DESTINATIONS`, `NUM_SUGGESTED`). Add new named constants for any new limits.

6. **Arithmetic on `INT_MAX` is forbidden without a guard.** Always check `dist[u] != INT_MAX` before adding to it.

7. **Keep the single-file structure.** Do not split into multiple `.c` files unless explicitly requested. New helper functions go above `main()` with a `//` comment describing their purpose.

8. **Compile cleanly at `-Wall -Wextra`.** No warnings are acceptable in submitted code.

9. **clang-format is applied automatically** (PostToolUse hook) to all `.c` and `.h` files on every Write/Edit. Do not manually reformat; let the hook handle it.

10. **Destructive shell commands are blocked** (`rm -rf`, `del /f`, `rmdir /s`) by the PreToolUse hook. Use safe alternatives or ask before proceeding.
