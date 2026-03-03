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
};

function $(id) {
  return document.getElementById(id);
}

function setStatus(text) {
  $("status").textContent = text;
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
  runBtn.disabled = true;

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
      body: JSON.stringify({ code, backend }),
    });

    const data = await resp.json();
    const elapsed = Math.round(performance.now() - started);

    $("rc").textContent = `rc: ${data.rc}`;
    $("time").textContent = `${data.duration_ms ?? elapsed} ms`;

    const outParts = [];
    if (data.out) outParts.push(data.out);
    if (data.stdout) outParts.push(data.stdout);
    $("out").textContent = outParts.join("") || "(no output)";

    const errParts = [];
    if (data.error) errParts.push(data.error + "\n");
    if (data.stderr) errParts.push(data.stderr);
    // If the tool wrote errors into the output file, surface them here too.
    if (data.rc !== 0 && data.out && !data.stderr) errParts.push(data.out);
    $("err").textContent = errParts.join("") || (data.rc === 0 ? "" : "(error)");

    setStatus(data.rc === 0 ? "Done" : "Done (with errors)");
  } catch (e) {
    $("err").textContent = String(e);
    setStatus("Failed");
  } finally {
    runBtn.disabled = false;
  }
}

window.addEventListener("DOMContentLoaded", () => {
  initEditor();
  $("run").addEventListener("click", runCode);
});
