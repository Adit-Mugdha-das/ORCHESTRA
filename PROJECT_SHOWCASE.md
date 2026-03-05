# ORCHESTRA Compiler Project — Showcase Overview

This repo contains a small programming language (**ORCHESTRA**) implemented with a classic **Flex/Bison** frontend and **multiple execution backends**:

- **AST interpreter** (reference semantics)
- **VM backend** (AST → stack bytecode → VM)
- **AST → “Equivalent C++”** pretty-printer (learning/debug view only)

It also includes a local **Web UI playground** that can run either backend and show the equivalent C++ translation.

---

## What you can demo quickly

1. **Build the compiler**: run [build_cpp.bat](build_cpp.bat)
2. **Run programs (AST backend)**: `orchestra.exe input.txt output.txt`
  - One-command runner: [run_cpp.bat](run_cpp.bat)
3. **Run programs (VM backend)**: `orchestra.exe input.txt output.txt --backend=vm`
4. **Show “Equivalent C++”** (no execution): `orchestra.exe input.txt output.txt --emit=cpp`
5. **Open the Web UI**: run [run_webui.bat](run_webui.bat)
6. **Prove backend equivalence** (AST vs VM): run [run_vm_tests.bat](run_vm_tests.bat)

---

## Language features (unique feature list)

### Program structure
- `flow name take(a,b,...) { ... }` defines a function (“flow”)
- `flow main take() { ... }` is the typical entrypoint

### Variables and scope
- `note x = expr;` declares a variable in the current scope
- `stage x = expr;` assigns to an existing variable
- `{ ... }` creates a new lexical scope (shadowing works)

### Control flow
- `branch(cond) { ... } elsewise { ... }` (if/else)
- `repeat(cond) { ... }` (while loop)
- `break;` and `continue;`
- `return expr;` / `return;`

### Expressions and operators
- Literals: `int`, `float`, `bool`, `string`
- Arithmetic: `+ - * /`, unary `-`
- Logic: `&& ||`, unary `!`
- Comparisons: `< <= > >= == !=`
- **Comparison chaining**: `1 < 2 < 3` evaluates as a chain (not C/C++)

### Collections (Milestone 9)
- Array literal: `[1,2,3]`
- Indexing:
  - arrays: `a[i]`
  - maps: `m["key"]`
  - sets: `s["key"]` → boolean membership
- Index assignment: `stage a[i] = expr;` and `stage m["k"] = expr;`

Built-in helpers (used consistently in AST + VM):
- Arrays: `array(n)`, `push(arr,v)`, `pop(arr)`, `resize(arr,n)`
- Maps/Sets: `map()`, `set()`, `add(set,k)`, `has(mapOrSet,k)`, `get(map,k)`, `put(map,k,v)`, `del(mapOrSet,k)`, `keys(mapOrSet)`

### OOP-like features (type + methods + inheritance)
- `symphony TypeName extends Parent { ... }` defines a class-like namespace and registers fields
- `ensemble TypeName { ... }` defines a struct-like type (fields)
- Constructor-like call: `TypeName(...)` creates an instance
- Optional user constructor: `flow init take(...) { ... }` as `TypeName.init(...)`
- Field access: `obj.field`
- Field assignment: `stage obj.field = expr;` and `stage this.field = expr;`
- Method calls: `obj.method(args)`
- Method context variable: `this`
- Inheritance calls: `super.method(args)` and `super(args)`

---

## Architectural features (what makes the project interesting)

- **Two independent execution engines**:
  - AST interpreter is the reference.
  - VM backend compiles the same AST to bytecode and executes it.
- **Correctness harness**: a batch runner compares AST vs VM outputs using `fc`.
- **Third “backend”** (emit mode): AST → C++-style pretty printer.
- **Local web playground** that shells out to the same compiler executable.

---

## Where to look in the code

- Frontend (lexer/parser): [orchestra.l](orchestra.l), [orchestra.y](orchestra.y)
- AST + runtime API: [interpreter.h](interpreter.h), [interpreter.c](interpreter.c)
- VM runtime + bytecode: [bytecode.h](bytecode.h), [bytecode.cpp](bytecode.cpp)
- VM compiler (AST → bytecode): [vm_backend.cpp](vm_backend.cpp)
- “Equivalent C++” printer: [cpp_printer.h](cpp_printer.h), [cpp_printer.cpp](cpp_printer.cpp)
- Web UI + server: [webui/index.html](webui/index.html), [webui/app.js](webui/app.js), [webui_server.py](webui_server.py)

---

## Notes / constraints (honest limitations)

- `fixed`, `play`, and `score` are currently **reserved tokens** (lexed) but not parsed/used.
- The “Equivalent C++” output is **for learning only** and is not guaranteed to compile.
- Map/set keys use a string-key model (values are stringified for keying).
