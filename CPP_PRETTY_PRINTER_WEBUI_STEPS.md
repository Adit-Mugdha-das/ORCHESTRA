# Add “Show Equivalent C++” (AST → C++ pretty-printer) + Web UI button

This feature is **for learning/debugging only**.

- It must **not** change the behavior of existing backends (`--backend=ast|vm`).
- It must **not** run the generated C++.
- It must only **display** a readable C++-style translation of the parsed program.

---

## Outcome

After implementing, you will be able to:

1. Open the local Web UI (already in this repo).
2. Write ORCHESTRA code.
3. Click **Run** (AST or VM) to see output/errors.
4. Click **Show C++** to see a C++-style translation of the same input.

---

## High-level approach (safe)

Add a **separate emit mode** to `orchestra.exe`:

- Default behavior stays the same:
  - `orchestra.exe input.txt output.txt` runs AST backend (current behavior)
  - `orchestra.exe input.txt output.txt --backend=vm` runs VM backend
- New optional mode:
  - `orchestra.exe input.txt output_cpp.txt --emit=cpp`
    - Parses the input
    - **Does not execute**
    - Writes the C++-style output to `output_cpp.txt`

Then the Web UI server just calls the executable with `--emit=cpp` when the user clicks the button.

This is low risk because it reuses your existing parser and only adds a new “printer”.

---

## Step 1 — Add a C++ pretty-printer module

Create 2 new files:

- `cpp_printer.h`
- `cpp_printer.cpp`

### 1.1 Public API

Expose one entrypoint:

- `void dump_cpp_program(Stmt* mainBlock, FILE* out);`

Where `mainBlock` is the same `Stmt*` you already execute today.

### 1.2 Printer structure

Implement two recursive functions:

- `static void print_expr_cpp(FILE* out, Expr* e, int parentPrec);`
- `static void print_stmt_cpp(FILE* out, Stmt* s, int indent);`

Plus helpers:

- `static void indent(FILE* out, int n);`
- `static void print_string_escaped(FILE* out, const char* s);`

### 1.3 What to print (mapping)

Use a **C++-style** mapping. It does not need to compile, but must be readable.

**Statements**

- `note x = expr;` → `auto x = <expr>;`
- `stage x = expr;` → `x = <expr>;`
- `emit expr;` → `emit(<expr>);`
- `{ ... }` → `{ ... }` with indentation
- `branch(cond) { ... } elsewise { ... }` → `if (cond) { ... } else { ... }`
- `repeat(cond) { ... }` → `while (cond) { ... }`
- `break;` / `continue;` / `return expr;` → same
- `stage name[index] = expr;` → `name[index] = <expr>;`
- `stage base.field = expr;` and `stage this.field = expr;` → `<base>.field = <expr>;`

**Expressions**

- Literals: `int`, `float`, `true/false`, strings
- Variables: just the name
- Unary: `!x`, `-x`
- Binary arithmetic: `+ - * /`
- `&&` / `||` print directly
- Index: `a[i]`
- Array literal: `[a,b]` → something readable like `Array{a, b}` or `{a, b}`
- Calls:
  - `f(a,b)` → `f(a,b)`
  - `obj.m(a)` → `obj.m(a)`
  - Constructors like `Type(…)` → `Type(…)`
- `super.method(a)` → `/*super*/ super_method(a)` (or another readable form)
- `super(args)` ctor call → `/*super*/ super_ctor(args)`

**Comparison chaining (important)**

Your grammar supports chains like `1 < 2 < 3`.
C++ does not mean the same thing, so translate chains into conjunctions:

- `a < b < c` → `(a < b) && (b < c)`
- `a <= b >= c` → `(a <= b) && (b >= c)`
- Same idea for `==` and `!=` chains.

You already have `Expr.chain_first` and linked `ChainPart` nodes in `interpreter.h`.

### 1.4 Header (helpful context)

Start the output with a small header comment, e.g.:

```cpp
// ORCHESTRA → C++ (pretty-print)
// NOTE: for learning only, not guaranteed to compile
```

---

## Step 2 — Add `--emit=cpp` to the CLI (without breaking existing)

Edit the argument parsing in the `main()` inside `orchestra.y`.

### Requirements

- Keep current behavior unchanged when `--emit` is absent.
- If `--emit=cpp` is present:
  - Parse input
  - Call `dump_cpp_program(g_main_block, outputFile)`
  - Exit with `0` on success
  - If parsing failed, keep existing error behavior (you already print `Syntax Error: ...`).

### Suggested parsing

Your CLI already supports:

- `orchestra.exe input output --backend=vm`

Extend it to accept:

- `--emit=cpp`
- or `--emit cpp`

If both `--backend` and `--emit=cpp` appear, **prefer emit** (because you’re not executing).

---

## Step 3 — Wire it into the build

Update `build_cpp.bat` to compile/link the new module:

- add:
  - `g++ -c cpp_printer.cpp -o cpp_printer.o || exit /b 1`
- link it in the final `g++ ... -o orchestra.exe` command.

This should not change runtime performance because it only adds a small object file.

---

## Step 4 — Add a Web UI button + endpoint

You already have a working Web UI (`webui/`) + local server (`webui_server.py`).

### 4.1 Add a button

In `webui/index.html`:

- Add a second button next to **Run**, labeled **Show C++**.

### 4.2 Add client behavior

In `webui/app.js`:

- Add a handler for **Show C++** that POSTs to the server.
- Display returned C++ in the “Output” panel.

Suggested payload:

```json
{ "code": "...", "action": "emit_cpp" }
```

### 4.3 Add a new API route

In `webui_server.py`:

- Extend `/api/run` to accept an `action` field:
  - `action=run` (default)
  - `action=emit_cpp`

When `action=emit_cpp`, run:

- `orchestra.exe input.txt output.txt --emit=cpp`

Return JSON:

- `rc`
- `out` (the pretty-printed C++ text)
- `stderr` / `stdout` (for errors)

---

## Step 5 — Quick manual tests (non-destructive)

1. Run a known-good program and click **Show C++**
   - Confirm it prints readable C++ and preserves structure (ifs/loops/returns, etc.)
2. Test a syntax error
   - Confirm errors show in the UI (don’t crash the server)
3. Test comparison chaining
   - Input: `emit 1 < 2 < 3;`
   - Confirm emitted C++ shows `(1 < 2) && (2 < 3)`

---

## Notes (keep it safe)

- Do **not** modify interpreter/VM runtime logic.
- Do **not** add optimizations or new language behavior.
- The pretty-printer should be purely a *view* of the AST.
- If you encounter an AST node you don’t support yet, print a clear placeholder like:
  - `/* unsupported: EXPR_X */`

---

## Optional polish (still cheap)

- Add a toggle in the UI for emitted style:
  - “C++-like” vs “Pseudo C++”
- Show line/indentation nicely.
- Include a short legend comment mapping ORCHESTRA keywords to the printed C++ forms.
