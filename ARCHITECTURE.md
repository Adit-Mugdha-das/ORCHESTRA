# ORCHESTRA — Architecture

This document explains how the project is structured internally and how the **same source program** can run through:

- an **AST interpreter** (reference)
- a **bytecode VM backend** (compiler + VM)
- a **C++ pretty-printer** (display-only)

---

## 1) Frontend: Lexing + Parsing

### Lexer (Flex)
- File: [orchestra.l](orchestra.l)
- Converts source text into tokens (keywords, operators, literals, identifiers)

### Parser (Bison)
- File: [orchestra.y](orchestra.y)
- Builds an AST using constructor helpers declared in [interpreter.h](interpreter.h)

Key AST nodes you’ll see:
- Expressions: literals, variables, calls, dot-calls, field access, unary/binary ops, chained comparisons, arrays, indexing, address-of (`&x`), dereference (`deref(p)`)
- Statements: declaration/assignment, emit, return, blocks, branch/else, repeat/for loops, break/continue, field/index assignment, pointer write-through (`stagethru`)

The parser also:
- registers `flow` definitions into a function registry
- tracks a “main block” entry flow

---

## 2) Shared runtime model

### AST memory management
- The AST and many runtime objects use an arena allocator (`arena_strdup`, `arena_free_all`) exposed in [interpreter.h](interpreter.h)

### Values and data model
- Implemented in [interpreter.c](interpreter.c)
- Core value kinds include:
  - numeric (int/float), bool, string
  - arrays, maps, sets
  - struct instances (ensembles)
  - pointers (reference to a named variable, created with `&x`)

Collections are heap-backed and referenced via pointers stored in `Value`.

### Type metadata and instances
The project supports struct/class-like types:
- `ensemble` and `symphony` declarations register metadata (fields + optional parent)
- Instances are created via constructor-like calls (`TypeName(...)`)
- Inheritance uses a parent chain from `extends`

`this` and `super` are resolved using runtime type metadata.

---

## 3) Backend A: AST interpreter (reference semantics)

- Implementation: [interpreter.c](interpreter.c)
- Execution is a tree-walk:
  - `execute_program(root, out)` runs the selected main flow body
  - expressions are evaluated recursively
  - statements update a scoped symbol table

This backend is treated as the **source of truth** for language semantics.

Pointer support:
- `EXPR_ADDROF` stores the target variable name as a pointer value.
- `EXPR_DEREF` looks up the target variable by name at runtime.
- `STMT_STAGETHRU` resolves the pointer and writes to the target variable.

---

## 4) Backend B: VM backend (compiler + VM)

The VM backend is intentionally structured as two phases:

### 4.1 Compiler: AST → stack bytecode
- File: [vm_backend.cpp](vm_backend.cpp)
- Walks the AST and emits a stack-based instruction stream.
- Uses jump patching for control flow (`branch`, `repeat`, `break`, `continue`).
- Ensures flows are compiled and can be called (supports recursion).

### 4.2 Runtime: bytecode execution loop
- Files: [bytecode.h](bytecode.h), [bytecode.cpp](bytecode.cpp)
- Executes opcodes against a value stack and call frames.
- Implements collection ops + indexing/get/set.

Design goal:
- match interpreter behavior first (correctness), then optimize.

Pointer support (opcodes):
- `ADDROF` — pushes a pointer value holding the target variable name.
- `DEREF_OP` — reads the current value of the pointed-to variable.
- `STORE_THRU` — writes through the pointer to the target variable.

---

## 5) Backend C: AST → “Equivalent C++” (pretty-printer)

- Files: [cpp_printer.h](cpp_printer.h), [cpp_printer.cpp](cpp_printer.cpp)
- Triggered by emit mode (see below)
- Produces readable, C++-style code for learning/debugging

Important behavior:
- This is **not** a code generator and does **not** execute the output.
- It rewrites chained comparisons (`a < b < c`) into `&&` form because C++ would interpret that differently.

Emit styles:
- `--emit-style=cpp` (C++-like): uses `auto` and `emit(expr);`
- `--emit-style=pseudo` (pseudo-C++): uses `var` and `print(expr);`

---

## 6) CLI orchestration

Entry point is in [orchestra.y](orchestra.y) `main()`.

Supported modes:
- Execute (default):
  - `orchestra.exe input.txt output.txt` → AST backend
  - `orchestra.exe input.txt output.txt --backend=vm` → VM backend
- Emit-only (no execution):
  - `orchestra.exe input.txt output.txt --emit=cpp`
  - optionally: `--emit-style=cpp|pseudo`

If both `--backend` and `--emit=cpp` are provided, emit mode is preferred (it does not execute).

---

## 7) Web UI architecture

The web UI is a thin wrapper around the same executable.

- Static UI: [webui/index.html](webui/index.html), [webui/styles.css](webui/styles.css), [webui/app.js](webui/app.js)
- Local server: [webui_server.py](webui_server.py)

Flow:
1. Browser sends JSON `{ code, backend, action }` to `/api/run`
2. Server writes a temp input file
3. Server runs `orchestra.exe` with either:
   - `--backend=ast|vm` for execution
   - `--emit=cpp --emit-style=...` for C++ printing
4. Server returns `{ rc, out, stdout, stderr }`

The UI caches the last Run output and last C++ output so **Show C++** can toggle on/off without losing the previous run output.

---

## 8) Correctness and testing strategy

There are two levels of tests:

- “Run tests” (single backend): [run_tests.bat](run_tests.bat)
- “VM equivalence” tests (AST vs VM output diff): [run_vm_tests.bat](run_vm_tests.bat)

Equivalence is validated by running the same input in both backends and comparing output files using Windows `fc`.

Manual emit tests:
- [run_emit_cpp_tests.bat](run_emit_cpp_tests.bat)

---

## 9) Build pipeline (Windows)

- Script: [build_cpp.bat](build_cpp.bat)
- Steps:
  - `bison -d orchestra.y`
  - `flex orchestra.l`
  - compile C parser/lexer with `gcc`
  - compile runtime + VM + printer with `g++`
  - link into `orchestra.exe`

Dependencies you need on PATH:
- Flex, Bison
- GCC/G++ (MinGW-w64 is a common option)
- Python 3 (only for the Web UI server)
