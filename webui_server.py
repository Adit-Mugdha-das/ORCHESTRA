import json
import os
import subprocess
import tempfile
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse

ROOT = Path(__file__).resolve().parent
WEBUI_DIR = ROOT / "webui"
ORCH_EXE = Path(os.environ.get("ORCH_EXE", str(ROOT / "orchestra.exe")))
HOST = os.environ.get("ORCH_HOST", "127.0.0.1")
PORT = int(os.environ.get("ORCH_PORT", "8000"))
MAX_CODE_BYTES = int(os.environ.get("ORCH_MAX_CODE_BYTES", str(200_000)))
RUN_TIMEOUT_SEC = float(os.environ.get("ORCH_RUN_TIMEOUT_SEC", "6"))


def _read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except FileNotFoundError:
        return ""


class Handler(BaseHTTPRequestHandler):
    server_version = "ORCHESTRA-WebUI/1.0"

    def log_message(self, fmt, *args):
        # Keep console noise low.
        return

    def _send(self, status: int, body: bytes, content_type: str) -> None:
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def _send_json(self, status: int, obj: dict) -> None:
        body = json.dumps(obj).encode("utf-8")
        self._send(status, body, "application/json; charset=utf-8")

    def do_GET(self):
        parsed = urlparse(self.path)
        path = parsed.path

        if path == "/" or path == "":
            index = WEBUI_DIR / "index.html"
            self._send(200, index.read_bytes(), "text/html; charset=utf-8")
            return

        if path.startswith("/"):
            path = path[1:]

        # Only serve files from webui/
        safe_path = (WEBUI_DIR / path).resolve()
        if WEBUI_DIR not in safe_path.parents and safe_path != WEBUI_DIR:
            self._send_json(404, {"error": "Not found"})
            return

        if not safe_path.exists() or not safe_path.is_file():
            self._send_json(404, {"error": "Not found"})
            return

        ext = safe_path.suffix.lower()
        ctype = {
            ".css": "text/css; charset=utf-8",
            ".js": "application/javascript; charset=utf-8",
            ".html": "text/html; charset=utf-8",
            ".map": "application/json; charset=utf-8",
            ".png": "image/png",
            ".svg": "image/svg+xml",
        }.get(ext, "application/octet-stream")

        self._send(200, safe_path.read_bytes(), ctype)

    def do_POST(self):
        parsed = urlparse(self.path)
        if parsed.path != "/api/run":
            self._send_json(404, {"error": "Not found"})
            return

        if not ORCH_EXE.exists():
            self._send_json(500, {"error": f"Missing compiler executable: {ORCH_EXE}"})
            return

        try:
            length = int(self.headers.get("Content-Length", "0"))
        except ValueError:
            length = 0

        if length <= 0 or length > (MAX_CODE_BYTES * 2):
            self._send_json(400, {"error": "Invalid request size"})
            return

        raw = self.rfile.read(length)
        if len(raw) > MAX_CODE_BYTES * 2:
            self._send_json(400, {"error": "Request too large"})
            return

        try:
            payload = json.loads(raw.decode("utf-8"))
        except Exception:
            self._send_json(400, {"error": "Invalid JSON"})
            return

        action = payload.get("action", "run")
        if action not in ("run", "emit_cpp"):
            action = "run"

        code = payload.get("code", "")
        backend = payload.get("backend", "ast")
        emit_style = payload.get("emitStyle", "cpp")
        if emit_style not in ("cpp", "pseudo"):
            emit_style = "cpp"
        if backend not in ("ast", "vm"):
            backend = "ast"

        if not isinstance(code, str):
            self._send_json(400, {"error": "code must be a string"})
            return

        if len(code.encode("utf-8", errors="ignore")) > MAX_CODE_BYTES:
            self._send_json(400, {"error": f"Code too large (max {MAX_CODE_BYTES} bytes)"})
            return

        started = time.perf_counter()

        with tempfile.TemporaryDirectory(prefix="orchestra_webui_") as td:
            td_path = Path(td)
            in_path = td_path / "input.txt"
            out_path = td_path / "output.txt"
            in_path.write_text(code, encoding="utf-8", errors="replace")

            if action == "emit_cpp":
                cmd = [str(ORCH_EXE), str(in_path), str(out_path), "--emit=cpp", f"--emit-style={emit_style}"]
            else:
                cmd = [str(ORCH_EXE), str(in_path), str(out_path), "--backend", backend]

            try:
                proc = subprocess.run(
                    cmd,
                    cwd=str(ROOT),
                    capture_output=True,
                    text=True,
                    timeout=RUN_TIMEOUT_SEC,
                )
                rc = proc.returncode
                stdout = proc.stdout or ""
                stderr = proc.stderr or ""
            except subprocess.TimeoutExpired:
                self._send_json(
                    200,
                    {
                        "rc": 124,
                        "out": _read_text(out_path),
                        "stdout": "",
                        "stderr": "Execution timed out",
                        "duration_ms": int((time.perf_counter() - started) * 1000),
                    },
                )
                return
            except Exception as e:
                self._send_json(500, {"error": str(e)})
                return

            out_text = _read_text(out_path)
            duration_ms = int((time.perf_counter() - started) * 1000)

            self._send_json(
                200,
                {
                    "rc": rc,
                    "out": out_text,
                    "stdout": stdout,
                    "stderr": stderr,
                    "duration_ms": duration_ms,
                },
            )


def main() -> None:
    if not WEBUI_DIR.exists():
        raise SystemExit("Missing webui/ directory")

    httpd = ThreadingHTTPServer((HOST, PORT), Handler)
    print(f"ORCHESTRA Web UI: http://{HOST}:{PORT}")
    print(f"Using: {ORCH_EXE}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
