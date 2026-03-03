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

const state = {
  editor: null,
  useFallback: false,
  activeView: "run", // 'run' | 'cpp'
  lastRunView: null,
  lastCppView: null,
  lastCppStyle: null,
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

function getCode() {
  if (state.useFallback) return $("fallback").value;
  return state.editor.getValue();
}

function setCode(code) {
  if (state.useFallback) {
    $("fallback").value = code;
  } else {
    state.editor.setValue(code, -1);
  }
}

function initEditor() {
  const editorEl = $("editor");
  const fallbackEl = $("fallback");

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
    });

    state.editor = ed;
    state.useFallback = false;
    fallbackEl.style.display = "none";
    editorEl.style.display = "block";
    setCode(DEFAULT_CODE);
    return;
  } catch {
    // Fallback for offline/no CDN.
    state.useFallback = true;
    editorEl.style.display = "none";
    fallbackEl.style.display = "block";
    fallbackEl.value = DEFAULT_CODE;
    return;
  }
}

async function runCode() {
  const runBtn = $("run");
  const showBtn = $("showCpp");
  runBtn.disabled = true;
  if (showBtn) showBtn.disabled = true;

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
    return;
  }

  state.activeView = "cpp";
  setShowCppButtonState();

  await emitCppAndShow();
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
  const styleSel = $("emitStyle");
  if (styleSel) styleSel.addEventListener("change", refreshCppIfVisible);
  setShowCppButtonState();
});
