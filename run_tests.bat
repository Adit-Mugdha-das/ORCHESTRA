@echo off
setlocal
cd /d "%~dp0"

call build_cpp.bat || exit /b 1

set FAIL=0

call :run_one test_return_call.txt out_test_return_call.txt || set FAIL=1
call :run_one test_comparison_chaining_eqne.txt out_test_chain_eqne.txt || set FAIL=1
call :run_one test_scopes_cpp_symtab.txt out_test_scopes.txt || set FAIL=1
call :run_one test_call_as_statement.txt out_test_call_stmt.txt || set FAIL=1
call :run_one test_recursion.txt out_test_recursion.txt || set FAIL=1
call :run_one test_class_namespace.txt out_test_class_namespace.txt || set FAIL=1
call :run_one test_inheritance.txt out_test_inheritance.txt || set FAIL=1

if %FAIL%==0 (
  echo.
  echo All tests executed.
  exit /b 0
) else (
  echo.
  echo One or more tests failed to run.
  exit /b 1
)

:run_one
set IN=%~1
set OUT=%~2
if not exist "%IN%" (
  echo.
  echo [SKIP] %IN% not found
  exit /b 0
)

echo.
echo [RUN] %IN%  ->  %OUT%
orchestra.exe "%IN%" "%OUT%"
set rc=%errorlevel%
if not %rc%==0 (
  echo [FAIL] rc=%rc%
  if exist "%OUT%" type "%OUT%"
  exit /b %rc%
)
if exist "%OUT%" type "%OUT%"
exit /b 0
