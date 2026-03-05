# ORCHESTRA — Run & Demo Guide (Windows)

This is a practical checklist you can follow right before presenting your project.

---

## 0) Prerequisites

Install and ensure these are in `PATH`:
- `bison` and `flex`
- `gcc` and `g++` (MinGW-w64)
- Python 3 (only needed for the Web UI)

---

## 1) Build

- Run: [build_cpp.bat](build_cpp.bat)
- Output binary: `orchestra.exe`

---

## 2) Run a program (AST backend)

Default backend is AST:

- `orchestra.exe input.txt output.txt`

One-command runner:
- [run_cpp.bat](run_cpp.bat)

You can use the included [input.txt](input.txt) or any `test_*.txt`.

---

## 3) Run a program (VM backend)

- `orchestra.exe input.txt output.txt --backend=vm`

One-command runner:
- [run_vm.bat](run_vm.bat)

---

## 4) Prove VM correctness (AST vs VM)

Runs multiple inputs through both backends and checks output equivalence:

- [run_vm_tests.bat](run_vm_tests.bat)

How it works:
- produces `out_*_ast.txt` and `out_*_vm.txt`
- compares them with `fc`

---

## 5) Show “Equivalent C++” (no execution)

Emit-only mode:

- `orchestra.exe input.txt output_cpp.txt --emit=cpp`

Optional style:
- `--emit-style=cpp` (C++-like)
- `--emit-style=pseudo` (pseudo-C++)

Example:
- `orchestra.exe manual_emitcpp_good.txt out_cpp.txt --emit=cpp --emit-style=pseudo`

Manual emit test runner:
- [run_emit_cpp_tests.bat](run_emit_cpp_tests.bat)

---

## 6) Run the Web UI

- Run: [run_webui.bat](run_webui.bat)
- Then open: `http://127.0.0.1:8000`

In the UI you can:
- choose backend: AST or VM
- click **Run** to execute
- click **Show C++** to display the C++-style view
- choose **C++ Style**: “C++-like” or “Pseudo C++”

Show C++ behavior:
- acts as a toggle
- hiding C++ restores the previous Run output (no re-run)

---

## 7) Feature demo script (suggested order)

### A) Control flow + scopes
Use a simple program:
- `note` declaration
- shadowing in `{ ... }`
- `repeat(...) { ... }`
- `branch(...) { ... } elsewise { ... }`
- `continue` / `return`

### B) Collections
Show:
- array literal + `stage a[i] = ...`
- `map()` and `stage m["k"] = ...`
- `set()` and `stage s["k"] = true;` (membership)
- `keys(...)` to list keys

### C) OOP / inheritance
Show:
- `symphony Child extends Parent { ... }`
- `this` inside methods
- `super.method(...)`
- `super(args)` constructor chaining

### D) Architecture proof
- Run AST and VM on the same program and show identical output
- Run the VM diff suite: [run_vm_tests.bat](run_vm_tests.bat)

### E) Learning feature
- In Web UI, click **Show C++** and explain:
  - it’s the parsed AST formatted into readable C++ style
  - chained comparisons become `&&` comparisons

---

## 8) Handy inputs already in the repo

- VM equivalence tests:
  - [test_vm_exprs.txt](test_vm_exprs.txt)
  - [test_vm_control_flow.txt](test_vm_control_flow.txt)
  - [test_vm_calls.txt](test_vm_calls.txt)
  - [test_vm_recursion.txt](test_vm_recursion.txt)
  - [test_vm_shadowing.txt](test_vm_shadowing.txt)

- OOP tests:
  - [test_class_namespace.txt](test_class_namespace.txt)
  - [test_inheritance.txt](test_inheritance.txt)

- Collections tests:
  - [test_arrays.txt](test_arrays.txt)
  - [test_map.txt](test_map.txt)
  - [test_set.txt](test_set.txt)

- Emit C++ manual tests:
  - [manual_emitcpp_good.txt](manual_emitcpp_good.txt)
  - [manual_emitcpp_chain.txt](manual_emitcpp_chain.txt)
  - [manual_emitcpp_syntax_error.txt](manual_emitcpp_syntax_error.txt)
