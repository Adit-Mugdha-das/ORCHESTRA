@echo off
setlocal
cd /d "%~dp0"

if "%~1"=="" goto :usage
if "%~2"=="" goto :usage

call build_cpp.bat || exit /b 1

orchestra.exe "%~1" "%~2" --backend=vm
set rc=%errorlevel%

if exist "%~2" (
  echo.
  echo ----- %~2 -----
  type "%~2"
)

exit /b %rc%

:usage
echo Usage: run_vm.bat ^<input.txt^> ^<output.txt^>
echo Example: run_vm.bat test_arrays.txt out_vm.txt
exit /b 2
