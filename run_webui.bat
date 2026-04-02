@echo off
setlocal
cd /d "%~dp0"

call build_cpp.bat || exit /b 1

set ORCH_PORT=8000
for /f %%P in ('powershell -NoProfile -Command "$ports = 8000..8010; foreach ($port in $ports) { try { $listener = [System.Net.Sockets.TcpListener]::new([System.Net.IPAddress]::Parse('127.0.0.1'), $port); $listener.Start(); $listener.Stop(); Write-Output $port; break } catch {} }"') do set ORCH_PORT=%%P

if "%ORCH_PORT%"=="" (
	echo.
	echo [ERROR] No free port found in range 8000-8010.
	exit /b 1
)

echo.
if not "%ORCH_PORT%"=="8000" (
	echo Port 8000 is busy. Using http://127.0.0.1:%ORCH_PORT% instead.
	echo.
)
echo Starting ORCHESTRA Web UI on http://127.0.0.1:%ORCH_PORT%
echo Close this window (or press Ctrl+C) to stop.

start "ORCHESTRA Web UI" powershell -NoProfile -Command "Start-Sleep -Seconds 1; Start-Process 'http://127.0.0.1:%ORCH_PORT%/'"

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
