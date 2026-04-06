const DEFAULT_CODE = `flow main take() {
  note a = [1,2,3];
  stage a[1] = 9;
  emit a;

  note m = map();
  stage m["name"] = "Ali";
  emit m;
  emit keys(m);

  note s = set();
  add(s, "x");
  stage s["y"] = true;
  emit s;
}
`;

const EXAMPLES = {
  basics: {
    label: "Variables & Output",
    code: `/* Variables & Output — demonstrates note, fixed, stage,
   emit, scientific notation, and both comment styles */
flow main take() {
  note x = 42;
  fixed PI = 3.14159;
  note msg = "Hello, ORCHESTRA!";

  // scientific notation literals
  note big   = 1e9;
  note small = 2.5e-3;

  emit msg;
  emit x;
  emit PI;
  emit big;
  emit small;

  stage x = x * 2;
  emit x;

  note flag = true;
  emit flag;
}
`,
  },

  control: {
    label: "Control Flow",
    code: `// Control Flow — branch/elsewise, &&/||/!, repeat, score, break, continue
flow main take() {
  note x = 7;
  note y = 3;

  // &&, ||, ! operators
  branch (x > 5 && y < 10) {
    emit "both conditions true";
  }
  branch (x < 0 || y > 0) {
    emit "at least one true";
  }
  branch (!false) {
    emit "NOT false is true";
  }

  // repeat loop with continue
  note i = 0;
  repeat (i < 4) {
    stage i = i + 1;
    branch (i == 3) { continue; }
    emit i;
  }

  // score loop with all three parts and break
  score (note j = 0; j < 5; stage j = j + 1) {
    branch (j == 4) { break; }
    emit j;
  }

  // score with empty init part
  note k = 3;
  score (; k > 0; stage k = k - 1) {
    emit k;
  }
}
`,
  },

  functions: {
    label: "Functions & Recursion",
    code: `// Flows, return values, and recursive Fibonacci
flow fibonacci take(n) {
  branch (n <= 1) { return n; }
  return fibonacci(n - 1) + fibonacci(n - 2);
}

flow greet take(name) {
  emit "Hello, " + name + "!";
}

flow main take() {
  greet("World");

  note i = 0;
  repeat (i <= 7) {
    emit fibonacci(i);
    stage i = i + 1;
  }
}
`,
  },

  collections: {
    label: "Collections (Array / Map / Set)",
    code: `// Collections — array literal, array(n), push/pop/resize, map, set
flow main take() {
  // --- Array literal ---
  note lit = [10, 20, 30];
  emit lit;

  // --- array(n) + push / pop / resize ---
  note arr = array(3);
  stage arr[0] = 10;
  stage arr[1] = 20;
  stage arr[2] = 30;
  push(arr, 40);
  emit arr;
  emit pop(arr);
  resize(arr, 6);
  emit arr;

  // --- Map ---
  note m = map();
  put(m, "lang", "ORCHESTRA");
  put(m, "year", "2026");
  emit get(m, "lang");
  emit has(m, "year");
  del(m, "year");
  emit keys(m);

  // --- Set ---
  note s = set();
  add(s, "a");
  add(s, "b");
  add(s, "a");       // duplicate — ignored
  emit s;
  emit has(s, "a");
  emit has(s, "z");
}
`,
  },

  oop: {
    label: "OOP & Inheritance",
    code: `// OOP — symphony, extends, this, super(), super.method()
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
    super(a);           // super constructor chaining
  }

  flow bark take() {
    super.describe();   // super.method() call
    emit this.age + 1;
  }
}

flow main take() {
  // subclass with super() and super.method()
  note d = Dog(5);
  d.bark();
}
`,
  },

  pointers: {
    label: "Pointers",
    code: `// Address-of, dereference, and write-through
flow main take() {
  note x = 10;
  note p = &x;

  emit deref(p);       // prints 10

  stagethru p = 99;
  emit x;              // prints 99 (x was updated through pointer)

  note result = deref(p) + 1;
  emit result;         // prints 100

  // Pointers work with all scalar types
  note msg = "hello";
  note q = &msg;
  stagethru q = "world";
  emit msg;            // prints world
}
`,
  },

  scoping: {
    label: "Scoping & Shadowing",
    code: `// Lexical scoping — inner blocks can shadow outer variables
flow double take(x) {
  return x * 2;
}

flow main take() {
  note x = 10;
  emit x;          // 10 — outer x

  {
    note x = 99;   // shadows outer x inside this block
    emit x;        // 99 — inner x

    {
      note x = 42; // shadows again one level deeper
      emit x;      // 42
    }

    emit x;        // 99 — inner x is restored
  }

  emit x;          // 10 — outer x is unchanged

  // flow parameters are their own scope
  emit double(x);  // 20
  emit x;          // still 10
}
`,
  },

  chaining: {
    label: "Comparison Chaining",
    code: `// ORCHESTRA supports chained comparisons as true logical chains
flow main take() {
  note a = 1;
  note b = 2;
  note c = 3;

  // All of these evaluate correctly as chains
  emit 1 < 2 < 3;          // true
  emit 3 > 2 > 1;          // true
  emit a < b < c;           // true
  emit a == 1 != 0;         // true
  emit 5 < 4 < 10;          // false — 5 < 4 is false

  branch (a < b < c) {
    emit "chain holds";
  }
}
`,
  },

  formatted: {
    label: "Formatted Output",
    code: `// play with format specifiers: %d %f %s %b
flow main take() {
  note age    = 21;
  note score  = 98.6;
  note name   = "ORCHESTRA";
  note passed = true;

  play "Name:   %s\\n", name;
  play "Age:    %d\\n", age;
  play "Score:  %f\\n", score;
  play "Passed: %b\\n", passed;

  // play without \n — no automatic newline
  play "A";
  play "B";
  play "C";
  emit "";     // flush with newline
}
`,
  },
};

const state = {
  editor: null,
  useFallback: false,
  activeView: "run", // 'run' | 'cpp' | 'bytecode'
  lastRunView: null,
  lastCppView: null,
  lastCppStyle: null,
  lastBytecodeView: null,
};

function $(id) {
  return document.getElementById(id);
}

function setStatus(text) {
  $("status").textContent = text;
}

function applyView(view) {
  if (!view) {
    $("out").textContent = "";
    $("err").textContent = "";
    $("rc").textContent = "rc: —";
    $("time").textContent = "— ms";
    setStatus("Ready");
    return;
  }

  $("out").textContent = view.outText ?? "";
  $("err").textContent = view.errText ?? "";
  $("rc").textContent = `rc: ${view.rc}`;
  $("time").textContent = `${view.timeMs} ms`;
  setStatus(view.statusText ?? (view.rc === 0 ? "Done" : "Done (with errors)"));
}

function setShowCppButtonState() {
  const showBtn = $("showCpp");
  if (!showBtn) return;
  showBtn.textContent = state.activeView === "cpp" ? "Hide C++" : "Show C++";
}

function setShowBytecodeButtonState() {
  const btn = $("showBytecode");
  if (!btn) return;
  btn.textContent = state.activeView === "bytecode" ? "Hide Bytecode" : "Show Bytecode";
}

function getEmitStyle() {
  const sel = $("emitStyle");
  const v = sel ? sel.value : "cpp";
  return v === "pseudo" ? "pseudo" : "cpp";
}

async function refreshCppIfVisible() {
  if (state.activeView !== "cpp") return;
  // Force refresh without toggling off.
  state.lastCppView = null;
  await emitCppAndShow();
}

async function refreshBytecodeIfVisible() {
  if (state.activeView !== "bytecode") return;
  state.lastBytecodeView = null;
  await emitBytecodeAndShow();
}

function getCode() {
  if (state.useFallback) return $("fallback").value;
  return state.editor.getValue();
}

function setCode(code) {
  if (state.useFallback) {
    $("fallback").value = code;
    const gutter = $("fallbackGutter");
    const ta = $("fallback");
    if (gutter && ta) {
      const lineCount = (ta.value.match(/\n/g) || []).length + 1;
      let s = "";
      for (let i = 1; i <= lineCount; i++) s += i + "\n";
      gutter.textContent = s;
      gutter.scrollTop = ta.scrollTop;
    }
  } else {
    state.editor.setValue(code, -1);
  }
}

function initEditor() {
  const editorEl = $("editor");
  const fallbackEl = $("fallback");
  const fallbackWrapEl = $("fallbackWrap");
  const fallbackGutterEl = $("fallbackGutter");

  function updateFallbackGutter() {
    if (!fallbackEl || !fallbackGutterEl) return;
    const lineCount = (fallbackEl.value.match(/\n/g) || []).length + 1;
    let s = "";
    for (let i = 1; i <= lineCount; i++) s += i + "\n";
    fallbackGutterEl.textContent = s;
  }

  function syncFallbackScroll() {
    if (!fallbackEl || !fallbackGutterEl) return;
    fallbackGutterEl.scrollTop = fallbackEl.scrollTop;
  }

  try {
    if (!window.ace) throw new Error("Ace not loaded");

    const ed = window.ace.edit(editorEl);
    ed.setTheme("ace/theme/tomorrow_night");
    ed.session.setMode("ace/mode/plain_text");
    ed.session.setUseSoftTabs(true);
    ed.session.setTabSize(2);
    ed.setOptions({
      fontSize: "13px",
      showPrintMargin: false,
      wrap: true,
      highlightActiveLine: true,
      showGutter: true,
      showLineNumbers: true,
      displayIndentGuides: true,
    });

    ed.renderer.setShowGutter(true);

    state.editor = ed;
    state.useFallback = false;
    if (fallbackWrapEl) fallbackWrapEl.style.display = "none";
    editorEl.style.display = "block";
    setCode(DEFAULT_CODE);
    return;
  } catch {
    // Fallback for offline/no CDN.
    state.useFallback = true;
    editorEl.style.display = "none";
    if (fallbackWrapEl) fallbackWrapEl.style.display = "grid";
    fallbackEl.value = DEFAULT_CODE;
    updateFallbackGutter();
    syncFallbackScroll();
    fallbackEl.addEventListener("input", updateFallbackGutter);
    fallbackEl.addEventListener("scroll", syncFallbackScroll);
    return;
  }
}

async function runCode() {
  const runBtn = $("run");
  const showBtn = $("showCpp");
  const bcBtn = $("showBytecode");
  runBtn.disabled = true;
  if (showBtn) showBtn.disabled = true;
  if (bcBtn) bcBtn.disabled = true;

  // Only clear the visible panel; keep cached views for toggling.
  $("out").textContent = "";
  $("err").textContent = "";
  $("rc").textContent = "rc: —";
  $("time").textContent = "— ms";

  const backend = $("backend").value;
  const code = getCode();

  setStatus("Running…");
  const started = performance.now();

  try {
    const resp = await fetch("/api/run", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ code, backend, action: "run" }),
    });

    const data = await resp.json();
    const elapsed = Math.round(performance.now() - started);

    const timeMs = data.duration_ms ?? elapsed;

    const outTextCombined = (data.out || "") + (data.stdout || "");
    const errTextCombined = (data.error ? data.error + "\n" : "") + (data.stderr || "");

    const trimmedOut = (data.out || "").trimStart();
    const isSyntaxError = trimmedOut.startsWith("Syntax Error:");

    const extra = data.rc !== 0 && data.out && !data.stderr ? data.out : "";
    const view = {
      rc: data.rc,
      timeMs,
      outText: isSyntaxError ? "" : (outTextCombined || "(no output)"),
      errText: isSyntaxError
        ? (data.out || "")
        : ((errTextCombined + extra) || (data.rc === 0 ? "" : "(error)")),
      statusText: data.rc === 0 ? "Done" : "Done (with errors)",
    };

    state.lastRunView = view;
    if (state.activeView === "run") applyView(view);
  } catch (e) {
    $("err").textContent = String(e);
    setStatus("Failed");
  } finally {
    runBtn.disabled = false;
    if (showBtn) showBtn.disabled = false;
    if (bcBtn) bcBtn.disabled = false;
  }
}

async function showCpp() {
  // Toggle behavior:
  // - If currently showing C++, hide it and restore last Run output.
  // - If not showing C++, show emitted C++ (cached if available).
  if (state.activeView === "cpp") {
    state.activeView = "run";
    applyView(state.lastRunView);
    setShowCppButtonState();
    setShowBytecodeButtonState();
    return;
  }

  state.activeView = "cpp";
  setShowCppButtonState();
  setShowBytecodeButtonState();

  await emitCppAndShow();
}

async function showBytecode() {
  if (state.activeView === "bytecode") {
    state.activeView = "run";
    applyView(state.lastRunView);
    setShowCppButtonState();
    setShowBytecodeButtonState();
    return;
  }

  state.activeView = "bytecode";
  setShowCppButtonState();
  setShowBytecodeButtonState();

  await emitBytecodeAndShow();
}

async function emitBytecodeAndShow() {
  if (state.lastBytecodeView) {
    applyView(state.lastBytecodeView);
    return;
  }

  const runBtn = $("run");
  const showBtn = $("showCpp");
  const bcBtn = $("showBytecode");
  if (bcBtn) bcBtn.disabled = true;
  if (showBtn) showBtn.disabled = true;
  if (runBtn) runBtn.disabled = true;

  $("out").textContent = "";
  $("err").textContent = "";
  $("rc").textContent = "rc: —";
  $("time").textContent = "— ms";

  const code = getCode();

  setStatus("Emitting Bytecode…");
  const started = performance.now();

  try {
    const resp = await fetch("/api/run", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ code, action: "emit_bytecode", backend: "vm" }),
    });

    const data = await resp.json();
    const elapsed = Math.round(performance.now() - started);
    const timeMs = data.duration_ms ?? elapsed;

    const errText = (data.error ? data.error + "\n" : "") + (data.stderr || "");
    const outText = data.out || "";
    const looksLikeBytecode = outText.trimStart().startsWith("== Bytecode (VM) ==");

    const view = {
      rc: data.rc,
      timeMs,
      outText: outText || "(no output)",
      errText: looksLikeBytecode
        ? (errText || (data.rc === 0 ? "" : "(error)"))
        : ((errText ? errText + "\n" : "") + "Bytecode view not available from the running server. Stop the WebUI (Ctrl+C) and re-run run_webui.bat, then refresh the page."),
      statusText: data.rc === 0 ? "Done" : "Done (with errors)",
    };

    state.lastBytecodeView = view;
    if (state.activeView === "bytecode") applyView(view);
  } catch (e) {
    $("err").textContent = String(e);
    setStatus("Failed");
  } finally {
    if (bcBtn) bcBtn.disabled = false;
    if (showBtn) showBtn.disabled = false;
    if (runBtn) runBtn.disabled = false;
  }
}

async function emitCppAndShow() {
  const style = getEmitStyle();

  if (state.lastCppView && state.lastCppStyle === style) {
    applyView(state.lastCppView);
    return;
  }

  const runBtn = $("run");
  const showBtn = $("showCpp");
  if (showBtn) showBtn.disabled = true;
  if (runBtn) runBtn.disabled = true;

  $("out").textContent = "";
  $("err").textContent = "";
  $("rc").textContent = "rc: —";
  $("time").textContent = "— ms";

  const code = getCode();

  setStatus("Emitting C++…");
  const started = performance.now();

  try {
    const resp = await fetch("/api/run", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ code, action: "emit_cpp", emitStyle: style }),
    });

    const data = await resp.json();
    const elapsed = Math.round(performance.now() - started);
    const timeMs = data.duration_ms ?? elapsed;

    const trimmedOut = (data.out || "").trimStart();
    const isSyntaxError = trimmedOut.startsWith("Syntax Error:");
    const errText = (data.error ? data.error + "\n" : "") + (data.stderr || "");

    const view = {
      rc: data.rc,
      timeMs,
      outText: isSyntaxError ? "" : (data.out || "(no output)"),
      errText: isSyntaxError ? (data.out || "") : (errText || (data.rc === 0 ? "" : "(error)")),
      statusText: data.rc === 0 ? "Done" : "Done (with errors)",
    };

    state.lastCppView = view;
    state.lastCppStyle = style;
    if (state.activeView === "cpp") applyView(view);
  } catch (e) {
    $("err").textContent = String(e);
    setStatus("Failed");
  } finally {
    if (showBtn) showBtn.disabled = false;
    if (runBtn) runBtn.disabled = false;
  }
}

window.addEventListener("DOMContentLoaded", () => {
  initEditor();
  $("run").addEventListener("click", runCode);
  const showBtn = $("showCpp");
  if (showBtn) showBtn.addEventListener("click", showCpp);
  const bcBtn = $("showBytecode");
  if (bcBtn) bcBtn.addEventListener("click", showBytecode);
  const styleSel = $("emitStyle");
  if (styleSel) styleSel.addEventListener("change", refreshCppIfVisible);
  setShowCppButtonState();
  setShowBytecodeButtonState();

  // --- Examples dropdown ---
  const examplesBtn = $("examplesBtn");
  const examplesMenu = $("examplesMenu");

  if (examplesBtn && examplesMenu) {
    examplesBtn.addEventListener("click", (e) => {
      e.stopPropagation();
      examplesMenu.hidden = !examplesMenu.hidden;
    });

    document.querySelectorAll(".exampleItem").forEach((btn) => {
      btn.addEventListener("click", () => {
        const key = btn.dataset.key;
        const example = EXAMPLES[key];
        if (!example) return;
        setCode(example.code);
        examplesMenu.hidden = true;
        // Clear cached views so fresh run uses the new code.
        state.lastRunView = null;
        state.lastCppView = null;
        state.lastBytecodeView = null;
        state.activeView = "run";
        applyView(null);
        setShowCppButtonState();
        setShowBytecodeButtonState();
        setStatus("Ready");
      });
    });

    // Close menu when clicking anywhere outside.
    document.addEventListener("click", () => {
      examplesMenu.hidden = true;
    });
  }
});
