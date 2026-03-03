@echo off
setlocal
cd /d "%~dp0"

call build_cpp.bat || exit /b 1

set FAIL=0

call :compare_one test_vm_exprs.txt out_vm_exprs_ast.txt out_vm_exprs_vm.txt || set FAIL=1
call :compare_one test_vm_blocks.txt out_vm_blocks_ast.txt out_vm_blocks_vm.txt || set FAIL=1
call :compare_one test_vm_shadowing.txt out_vm_shadow_ast.txt out_vm_shadow_vm.txt || set FAIL=1
call :compare_one test_vm_control_flow.txt out_vm_cf_ast.txt out_vm_cf_vm.txt || set FAIL=1
call :compare_one test_vm_calls.txt out_vm_calls_ast.txt out_vm_calls_vm.txt || set FAIL=1
call :compare_one test_vm_recursion.txt out_vm_rec_ast.txt out_vm_rec_vm.txt || set FAIL=1

call :compare_one test_class_namespace.txt out_vm_oop_ns_ast.txt out_vm_oop_ns_vm.txt || set FAIL=1
call :compare_one test_inheritance.txt out_vm_oop_inh_ast.txt out_vm_oop_inh_vm.txt || set FAIL=1

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
