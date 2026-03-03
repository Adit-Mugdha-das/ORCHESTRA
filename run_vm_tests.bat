@echo off
setlocal
cd /d "%~dp0"

call build_cpp.bat || exit /b 1

set FAIL=0

call :compare_one test_vm_exprs.txt out_vm_exprs_ast.txt out_vm_exprs_vm.txt || set FAIL=1

if %FAIL%==0 (
  echo.
  echo VM comparison tests passed.
  exit /b 0
) else (
  echo.
  echo VM comparison tests FAILED.
  exit /b 1
)

:compare_one
set IN=%~1
set OUT_AST=%~2
set OUT_VM=%~3

echo.
echo [AST] %IN% -> %OUT_AST%
orchestra.exe "%IN%" "%OUT_AST%" --backend=ast
set rc=%errorlevel%
if not %rc%==0 (
  echo [FAIL] AST rc=%rc%
  if exist "%OUT_AST%" type "%OUT_AST%"
  exit /b %rc%
)

echo.
echo [VM ] %IN% -> %OUT_VM%
orchestra.exe "%IN%" "%OUT_VM%" --backend=vm
set rc=%errorlevel%
if not %rc%==0 (
  echo [FAIL] VM rc=%rc%
  if exist "%OUT_VM%" type "%OUT_VM%"
  exit /b %rc%
)

echo.
echo [DIFF] %OUT_AST% vs %OUT_VM%
fc "%OUT_AST%" "%OUT_VM%" >nul
if not %errorlevel%==0 (
  echo [FAIL] Output mismatch
  echo ---- AST ----
  type "%OUT_AST%"
  echo ---- VM ----
  type "%OUT_VM%"
  exit /b 1
)

echo [OK] Outputs match
exit /b 0
