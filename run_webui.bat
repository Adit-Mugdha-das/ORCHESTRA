@echo off
setlocal
cd /d "%~dp0"

call build_cpp.bat || exit /b 1

echo.
echo Starting ORCHESTRA Web UI on http://127.0.0.1:8000
echo Close this window (or press Ctrl+C) to stop.

start "ORCHESTRA Web UI" http://127.0.0.1:8000/

rem Find a Python command (py launcher isn't always installed)
set PY_CMD=
where py >nul 2>nul && set PY_CMD=py
if "%PY_CMD%"=="" (
	where python >nul 2>nul && set PY_CMD=python
)
if "%PY_CMD%"=="" (
	where python3 >nul 2>nul && set PY_CMD=python3
)

if "%PY_CMD%"=="" (
	echo.
	echo [ERROR] Python not found in PATH.
	echo Install Python 3.x and re-run this script.
	exit /b 1
)

%PY_CMD% webui_server.py
