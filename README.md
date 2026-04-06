# ORCHESTRA

**ORCHESTRA** is a custom programming language with a complete compiler pipeline built using Flex and Bison. The same source program can be executed through three independent backends — an AST interpreter, a stack-based virtual machine, and a C++ code emitter — all sharing a single frontend.

> Course: Compiler Design Laboratory (CSE 3212) — Khulna University of Engineering and Technology

---

## Pipeline

```
Source Code → Lexer (Flex) → Parser (Bison) → AST → AST Interpreter
                                                    → VM Compiler → Bytecode VM
                                                    → C++ Emitter
```

---

## Prerequisites

- `gcc` and `g++` (MinGW-w64)
- `flex` and `bison`
- Python 3 (only for the Web UI)

All tools must be in `PATH`.

---

## Build

```bat
build_cpp.bat
```

Produces `orchestra.exe`.

---

## Usage

| Command | Description |
|---|---|
| `orchestra.exe input.txt output.txt` | Run with AST backend (default) |
| `orchestra.exe input.txt output.txt --backend=vm` | Run with VM backend |
| `orchestra.exe input.txt output.txt --emit=cpp` | Emit equivalent C++ (no execution) |
| `orchestra.exe input.txt output.txt --emit=cpp --emit-style=pseudo` | Emit pseudo-C++ style |

### Quick runners

| Script | Purpose |
|---|---|
| `run_cpp.bat` | Run `input.txt` on AST backend |
| `run_vm.bat` | Run `input.txt` on VM backend |
| `run_tests.bat` | Run AST test suite |
| `run_vm_tests.bat` | Run AST vs VM equivalence tests |
| `run_webui.bat` | Start local Web UI at `http://127.0.0.1:8000` |

---

## Language at a Glance

ORCHESTRA uses its own keyword set. The table below maps each keyword to its C equivalent.

| ORCHESTRA | Meaning | C Equivalent |
|---|---|---|
| `flow` | Function definition | `function` / `void` |
| `take` | Parameters | `(params)` |
| `note` | Mutable variable | `auto` |
| `fixed` | Immutable constant | `const` |
| `stage` | Assignment | `=` |
| `emit` | Print with newline | `printf` + `\n` |
| `play` | Formatted print | `printf` |
| `branch` / `elsewise` | Conditional | `if` / `else` |
| `repeat` | While loop | `while` |
| `score` | For loop | `for` |
| `symphony` | Class with methods | `class` |
| `ensemble` | Struct (data only) | `struct` |

### Example Program

```orchestra
symphony Animal {
  int age;

  flow init take(a) {
    stage this.age = a;
  }

  flow describe take() {
    emit this.age;
  }
}

symphony Dog extends Animal {
  flow init take(a) {
    super(a);
  }

  flow bark take() {
    super.describe();
    emit this.age + 1;
  }
}

flow main take() {
  note d = Dog(5);
  d.bark();
}
```

**Output:**
```
5
6
```

---

## Features

- **Variables & scope** — `note`, `fixed`, `stage`; lexical scoping with shadowing
- **Control flow** — `branch`/`elsewise`, `repeat`, `score`, `break`, `continue`, `return`
- **Functions** — `flow name take(args){}` with recursion (limit: 200 frames)
- **Collections** — arrays `[1,2,3]`, maps `map()`, sets `set()` with built-in helpers (`push`, `pop`, `has`, `keys`, `get`, `put`, `del`)
- **OOP** — `symphony` (class + methods), `ensemble` (struct), `extends` (inheritance), `this`, `super()`, `super.method()`
- **Pointers** — `&x` (address-of), `deref(p)` (read), `stagethru p = v` (write-through)
- **Operators** — arithmetic, logical, comparison, unary; comparison chaining (`1 < 2 < 3`)
- **Literals** — int, float, bool, string (with escape sequences), scientific notation (`1e9`, `2.5e-3`)
- **Comments** — single-line `//` and multi-line `/* */`

---

## Architecture

| File | Role |
|---|---|
| `orchestra.l` | Flex lexer — tokenizes source |
| `orchestra.y` | Bison parser — builds AST, drives backend selection |
| `interpreter.h` | Shared AST node and value type definitions |
| `interpreter.c` | AST interpreter (reference backend) |
| `symbol_table.cpp` | Scoped symbol table for variables |
| `flow_registry.cpp` | Function definition registry |
| `vm_backend.cpp` | AST → stack bytecode compiler |
| `bytecode.cpp` | Bytecode VM executor |
| `cpp_printer.cpp` | AST → C++ source-to-source transpiler |
| `webui_server.py` | Python HTTP server wrapping `orchestra.exe` |
| `webui/` | Browser-based playground (HTML + JS + CSS) |

---

## Error Handling

Errors are caught at every stage and reported with context:

| Stage | Example | Message |
|---|---|---|
| Lexer | Invalid character | `Lexical Error: ... at line N` |
| Parser | Missing semicolon | `Syntax Error: ... at line N` |
| Runtime | Undefined variable | `Runtime Error: ...` |
| VM compile | Bad jump target | `VM Compile Error: ...` |
| VM runtime | Stack overflow | `VM Error: ...` |

---

## Testing

Backend equivalence is verified by running each test through both the AST interpreter and the VM, then comparing outputs with `fc`:

```bat
run_vm_tests.bat
```

Test categories: expressions, control flow, recursion, collections, OOP, pointers, scoping/shadowing.

All categories pass on both backends.

---

## Web UI

```bat
run_webui.bat
```

Open `http://127.0.0.1:8000` in a browser. The UI provides a code editor, backend selector (AST / VM), a **Run** button, and a **Show C++** toggle that displays the emitted C++ without re-executing the program.

---

## License

Academic project — Khulna University of Engineering and Technology, 2026.
