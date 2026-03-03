# ORCHESTRA IR + VM Plan (Stack Bytecode)

This document is a step-by-step plan to add an **IR (stack bytecode)** + **VM backend** to the current ORCHESTRA project **without removing** the existing AST interpreter.

The core idea is to run two backends side-by-side:

- **Backend A (reference):** AST tree-walk interpreter (current)
- **Backend B (new):** AST → Bytecode compiler → VM

You will add the VM in small milestones and **test each milestone**.

---

## Guiding Rules (to avoid breaking the project)

1. **Never delete the AST interpreter** while building the VM.
2. Add the VM behind a **backend switch** (`--backend=ast|vm`). Default stays `ast` until VM matches.
3. Always verify **output equivalence**:
   - `--backend=ast` output == `--backend=vm` output for the same input.
4. Keep the first VM version **unoptimized**. Correctness first.
5. Reuse your existing runtime `Value` representation initially (int/float/bool/string/struct/array/map/set).

---

## Milestone 0 — Backend switch (no VM yet)

### Implement
- Add an option to select backend:
  - `--backend=ast` (default)
  - `--backend=vm`
- Add two entrypoints:
  - `execute_program_ast(root, out)` → calls current `execute_program(root, out)`
  - `execute_program_vm(root, out)` → temporary stub: prints a clear error like `VM backend not implemented` and exits.

### Verify
- All existing tests still pass in AST mode.
- Running any test with `--backend=vm` fails fast with the expected message.

---

## Milestone 1 — Define the bytecode IR (data structures)

### Implement
Create a minimal IR representation:

- `enum OpCode` (see Milestone 2 for the initial set)
- `struct Instr { OpCode op; int a; int b; }`
- `struct BytecodeFunc { Instr* code; int code_len; /* const pool refs, local count, etc */ }`
- `struct BytecodeProgram { /* functions by name, constants pool */ }`

Keep it simple:
- Use a dynamic array (`std::vector<Instr>` if in C++, or a growable C array).
- Add helper `emit(op, a, b)` to append instructions.

### Verify
- Unit smoke: create a tiny bytecode function manually (hardcoded) and ensure it can be stored/printed/dumped.

---

## Milestone 2 — VM skeleton (instruction dispatch loop)

### Implement
Create a VM executor with:

- A value stack: `Value stack[STACK_MAX]` + `sp`
- A loop: `while (ip < code_len) switch(op) { ... }`
- A minimal instruction set to start:

**Core stack ops**
- `PUSH_INT k`
- `PUSH_FLOAT k`
- `PUSH_BOOL k`
- `PUSH_STR const_id`
- `POP`

**Arithmetic (numeric)**
- `ADD`, `SUB`, `MUL`, `DIV`
- `NEG`

**Comparison**
- `EQ`, `NE`, `LT`, `LE`, `GT`, `GE`

**Output**
- `EMIT` (pops 1 value and prints)

**Control flow**
- `JMP target`
- `JMP_IF_FALSE target` (pops 1 bool)

**Return**
- `RET` (optionally returns top-of-stack)

Notes:
- Don’t implement calls/frames yet.
- Keep string constants in a constant pool.

### Verify
- Add a VM-only smoke program (manual bytecode) that does:
  - `emit 1+2*3` and prints `7`
- Confirm output is correct.

---

## Milestone 3 — AST → Bytecode compiler for expressions (no statements)

### Implement
Write `compile_expr(Expr* e)` that emits bytecode producing one value on the stack.

Start with:
- literals: numbers, bools, strings
- unary: `!`, unary `-`
- binary: `+ - * /`, comparisons

Mapping pattern:
- For `a + b`:
  - compile `a` → pushes value
  - compile `b` → pushes value
  - emit `ADD`

### Verify
- Add a tiny test file with only `emit` statements (no vars):
  - `emit 1 + 2 * 3;`
  - `emit (10 - 3) / 7;`
- Compare `--backend=ast` vs `--backend=vm`.

---

## Milestone 4 — Compile sequential statements: `emit expr;` and blocks

### Implement
Add statement compilation for:
- `STMT_EMIT` → compile expr, emit `EMIT`
- `STMT_BLOCK` → compile statement list in order

You still do **no variables**.

### Verify
- Your existing tests that don’t rely on vars (or a dedicated one) match outputs.

---

## Milestone 5 — Variables + scopes (initially name-based)

### Implement
To minimize risk, start with **name-based locals** (slow but correct):

- VM frame holds a map/dictionary from name → `Value` (or reuse your current symbol table calls).
- Implement ops (or direct VM helpers) for:
  - `NOTE x = expr;`
  - `STAGE x = expr;`
  - `VAR x` reads

Then add scope stack:
- on `{ ... }` push scope
- on block end pop scope

### Verify
- Add tests:
  - shadowing: `note x=1; { note x=2; emit x; } emit x;`
- Compare AST vs VM.

### Upgrade later (performance)
Once correct, replace name-based lookup with **slot-based locals**:
- a compile-time local index table per function
- bytecode uses `LOAD_LOCAL i` / `STORE_LOCAL i`

---

## Milestone 6 — Branch + repeat + break/continue

### Implement
Compile control flow with jump patching:

- `branch(cond){...} elsewise {...}`
- `repeat(cond){...}`
- `break;` / `continue;`

You will need:
- jump patch lists
- a loop context stack storing:
  - `continue_target`
  - list of `break` jumps to patch

### Verify
- Loop tests with break/continue.
- Compare AST vs VM outputs.

---

## Milestone 7 — Flows (functions), calls, returns, recursion depth

### Implement
Compile each registered flow to bytecode:

- `BytecodeFunc` per flow name
- Call frames:
  - return IP
  - locals storage
  - (optional) `this` binding

Instructions:
- `CALL name_const_id argc`
- `RET`

Implement:
- argument passing
- `return expr;` and `return;` default
- recursion depth guard (mirror current behavior)

### Verify
- Call/return tests
- Recursion tests
- Compare AST vs VM.

---

## Milestone 8 — OOP runtime integration (this/super, dot calls)

### Implement
Match existing semantics:

- `obj.method(...)` implicit-this
- `this` variable inside method
- `super.method(...)`
- `super(args)` constructor chaining (if you keep it)

Strategy (low risk):
- Reuse the same method resolution logic you already have (type → parent chain lookup).
- VM call should support an implicit `this` value passed into the frame.

### Verify
- Existing OOP tests:
  - symphony/ensemble usage
  - inheritance
  - super calls

---

## Milestone 9 — Collections in VM: array / map / set

### Implement
Reuse your existing pointer-backed runtime objects:

- Arrays:
  - literal `[ ... ]`
  - indexing `a[i]` get
  - index assignment `stage a[i] = v;`
  - built-ins: `array`, `push`, `pop`, `resize`

- Map:
  - `map()`
  - `m["k"]` get/set
  - helpers: `has/get/put/del/keys`

- Set:
  - `set()`
  - `s["x"]` membership
  - `stage s["x"] = true/false`
  - helpers: `add/has/del/keys`

### Verify
- Compare existing collection tests for AST vs VM.

---

## Milestone 10 — Turn VM into the default (only after equivalence)

### Implement
- After all tests match, make `--backend=vm` the default.
- Keep `--backend=ast` for debugging.

### Verify
- Run all tests in both modes periodically.

---

## Optional performance milestones (after correctness)

### A) Slot-based locals
- Replace name-based lookups with local indices.

### B) Bytecode peephole
- Remove `PUSH; POP` pairs
- Fold constant arithmetic

### C) Inline caching for method lookup
- Cache resolved `Type.method` calls when stable.

---

## How to test each milestone

For every milestone:
1. Run once with AST backend
2. Run again with VM backend
3. Diff the outputs

If you don’t want to change CLI yet, you can also use an environment variable:
- `ORCH_BACKEND=ast|vm`

---

## Notes about correctness traps

- Ensure `repeat` semantics match (especially break/continue behavior).
- Ensure short-circuit `&&` and `||` match (VM may need dedicated compilation for short-circuiting, not just `AND`/`OR` ops).
- Ensure `==`/`!=` type rules match your interpreter.
- Ensure printing (`emit`) formats exactly the same (especially structs/arrays/maps/sets).

---

## Next: what you tell me to start implementing

When you’re ready to start coding the VM, tell me:

1. Flag style:
   - `--backend=vm` (recommended)
   - or env var `ORCH_BACKEND=vm`

2. Whether to compile **whole program** once, or compile each flow lazily on first call.

Then we’ll implement Milestone 0 → Milestone 1 → Milestone 2 and test after each.
