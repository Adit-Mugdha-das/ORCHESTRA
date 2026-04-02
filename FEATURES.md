# ORCHESTRA — Complete Feature List (Project + Language)

This file lists **only features** (no architecture walkthrough). It is intended as a “what my project can do” reference before you present.

---

## A) Project-level features

### A1) Multiple execution modes
- **AST interpreter backend (reference):** runs directly from the parsed AST.
- **VM backend:** compiles AST to stack bytecode and runs it on a VM.
- **Emit mode (learning/debug):** prints an “Equivalent C++” view of the AST (does not execute).

### A2) CLI features
- Run (defaults to AST):
  - `orchestra.exe input.txt output.txt`
- Choose backend:
  - `--backend=ast` or `--backend=vm`
  - `--backend ast` or `--backend vm`
- Emit-only mode:
  - `--emit=cpp` or `--emit cpp`
  - If `--emit=cpp` is present, it is preferred over `--backend` (emit prints; it does not run).
- Emit style (for emit mode):
  - `--emit-style=cpp` (C++-like)
  - `--emit-style=pseudo` (pseudo-C++)

### A3) One-command runners (batch files)
- Build: [build_cpp.bat](build_cpp.bat)
- Run AST quickly: [run_cpp.bat](run_cpp.bat)
- Run VM quickly: [run_vm.bat](run_vm.bat)
- Run AST-only test suite: [run_tests.bat](run_tests.bat)
- Run AST vs VM output equivalence suite: [run_vm_tests.bat](run_vm_tests.bat)
- Run emit-mode manual tests: [run_emit_cpp_tests.bat](run_emit_cpp_tests.bat)
- Start Web UI: [run_webui.bat](run_webui.bat)

### A4) Web UI features (local playground)
- Runs locally via [webui_server.py](webui_server.py)
- UI includes:
  - Editor (Ace via CDN, with `<textarea>` fallback if offline)
  - Backend dropdown: AST (reference) vs VM (bytecode)
  - **Run** button
  - **Show C++** button that acts as a **toggle**:
    - click once: shows emitted C++
    - click again: hides C++ and restores the previous Run output (no re-run)
  - C++ Style dropdown: **C++-like** vs **Pseudo C++**
  - Output pane + Errors/Diagnostics pane

---

## B) Language features (syntax + semantics)

### B1) Source format basics
- Whitespace is ignored (spaces/tabs/newlines).
- Single-line comments: `// ...` (to end-of-line).
- Identifiers: `[a-zA-Z][a-zA-Z0-9_]*`.
- Numeric literals:
  - int: `123`
  - float: `123.45`
  - scientific: `1e9`, `2.5e-3`, `1E+6` (exponent suffix on int or float)
- String literals:
  - written as `"..."`
  - lexer accepts backslash escapes in the token, but **does not unescape them** (the characters are preserved as typed).

### B2) Program units

#### B2.1) Functions (flows)
- Define a flow:
  - `flow name take(a,b,c) { ... }`
- Parameters are optional:
  - `flow name take() { ... }`
  - `flow name { ... }` (allowed; `take(...)` part can be omitted)
- Entry selection:
  - If a top-level flow named `main` exists, it is used.
  - Otherwise, the first top-level flow becomes the entry.

#### B2.2) Blocks and lexical scope
- Block:
  - `{ statement* }`
- Blocks are valid as standalone statements (nested blocks work).
- Each block introduces a new scope; shadowing works.

### B3) Statements

#### B3.1) Variable declaration
- `note x = expr;`

#### B3.2) Assignment
- Variable assignment:
  - `stage x = expr;`

#### B3.3) Output
- `emit expr;`

#### B3.4) Control flow
- If / else:
  - `branch(cond) { ... }`
  - `branch(cond) { ... } elsewise { ... }`
- While loop:
  - `repeat(cond) { ... }`
- Loop control:
  - `break;`
  - `continue;`

#### B3.5) Return
- `return expr;`
- `return;`

#### B3.6) Call statements
- Call a flow as a statement:
  - `f(a,b);`
- Dot-call as a statement:
  - `obj.method(a,b);`

#### B3.7) Index assignment (collections)
- `stage name[indexExpr] = valueExpr;`
  - `name` must be an identifier (not an arbitrary expression target)

#### B3.8) Field assignment (struct/class instances)
- `stage base.field = expr;`
- `stage this.field = expr;`
  - `base` is an identifier in the grammar (not an arbitrary expression)

#### B3.9) Super-constructor statement
- `super(args...);`
  - Parsed as an expression statement that calls the parent constructor.

---

## C) Expressions

### C1) Literals
- int / float / bool / string:
  - `123`, `3.14`, `true`, `false`, `"hello"`

### C2) Variables
- `x`

### C3) Parentheses
- `(expr)`

### C4) Unary operators
- `-expr`
- `!expr`

### C5) Binary operators
- Arithmetic: `+`, `-`, `*`, `/`
- Logic: `&&`, `||`
- Comparisons: `<`, `<=`, `>`, `>=`, `==`, `!=`

Operator precedence (high → low):
- unary `!` and unary `-`
- `*` and `/`
- `+` and `-`
- comparisons (`< <= > >= == !=`)
- `&&`
- `||`

### C6) Comparison chaining (important)
The grammar supports **comparison chains**:
- `1 < 2 < 3`
- `a == b != c`

Semantics:
- Evaluates left-to-right as a chain.
- For `< <= > >=`: operands must be numeric.
- For `== !=`: operands must be comparable (numbers with numbers, bool with bool, string with string).
- The entire chain returns `true` only if every link is true.

Emit-mode note:
- The “Equivalent C++” printer rewrites `a < b < c` into `((a < b) && (b < c))` because C++ would interpret chaining differently.

### C7) Function calls
- `f(a,b,c)`

### C8) Dot calls
- `obj.method(a,b)`
- `TypeName.method(a,b)` (also parsed as a dot call)

### C9) Field access
- `obj.field`
- `this.field`

### C10) Arrays
- Array literal:
  - `[a, b, c]`

### C11) Indexing (arrays, maps, sets)
- `target[indexExpr]`

Runtime meaning:
- If `target` is an array: returns the element at index (bounds-checked).
- If `target` is a map: returns the value for the stringified key.
- If `target` is a set: returns a boolean membership result.

### C12) `this` and `super`
- `this` is a valid expression (variable-like).
- `super.method(args)` is a valid expression.
- `super(args)` is a valid expression and also allowed as a statement (as `super(args);`).

---

## D) Collections: built-in helpers

These are provided as built-in flows/intrinsics and work in both AST and VM backends.

### D1) Arrays
- `array(n)` → create array of length `n`
- `push(arr, value)` → append; returns new length
- `pop(arr)` → remove and return last element
- `resize(arr, newSize)` → resize array; returns new length

### D2) Maps
- `map()` → create empty map
- `get(m, key)` → get value
- `put(m, key, value)` → set value, returns the value

### D3) Sets
- `set()` → create empty set
- `add(s, key)` → add key, returns size

### D4) Shared map/set helpers
- `has(container, key)` where `container` is map or set → bool
- `del(container, key)` where `container` is map or set → bool
- `keys(container)` where `container` is map or set → returns an array of string keys

Key model:
- Map/set keys are treated as **string keys** (values are stringified for keying).

---

## E) OOP-like features (types, fields, methods, inheritance)

### E1) Type declarations

#### E1.1) `ensemble` (struct-like)
- Declare fields:
  - `ensemble Point { int x; int y; }`

#### E1.2) `symphony` (class-like + namespace)
- Declares a type and opens a scope for method name-qualification:
  - `symphony Child extends Parent { ... }`
- Members supported inside a `symphony` body:
  - flow definitions (methods)
  - field declarations: `int name;`, `float f;`, `bool b;`, `string s;`

Supported field types in declarations:
- `int`, `float`, `bool`, `string`

### E2) Constructors / instances
- Constructor-like call expression:
  - `TypeName(args...)`
- If `flow init take(...) { ... }` exists as a method (`TypeName.init`), it is used as a user constructor.
- Otherwise, constructor args must match the field count (including inherited fields).

### E3) Inheritance
- `extends Parent` supported for `symphony`.
- `super.method(args)` supported.
- `super(args)` supported for constructor chaining.

### E4) Method calls and `this`
- Dot calls such as `obj.method(args)` are supported.
- `this` is available in method contexts.

---

## F) Pretty-printer (“Equivalent C++”) features

Triggered by:
- `--emit=cpp`

What it does:
- Parses the program and prints a readable, C++-style representation.
- Adds a header comment and a **legend mapping** ORCHESTRA keywords to printed forms.
- Has two styles:
  - `--emit-style=cpp`: uses `auto` and `emit(expr);`
  - `--emit-style=pseudo`: uses `var` and `print(expr);`

Safety:
- Emit mode does **not** execute AST or VM.

---

## G) Reserved / not-yet-used keywords

These are tokenized by the lexer but are not currently used in the grammar/semantics:
- `fixed`
- `play`
- `score`
