@echo off
setlocal
cd /d "%~dp0"

call build_cpp.bat || exit /b 1

echo.
echo ===== Test 1: known-good program (emit C++) =====
orchestra.exe manual_emitcpp_good.txt out_manual_emitcpp_good.cpp.txt --emit=cpp
echo.
type out_manual_emitcpp_good.cpp.txt

echo.
echo ===== Test 2: syntax error handling (emit C++) =====
orchestra.exe manual_emitcpp_syntax_error.txt out_manual_emitcpp_syntax_error.txt --emit=cpp
echo.
type out_manual_emitcpp_syntax_error.txt

echo.
echo ===== Test 3: comparison chaining (emit C++) =====
orchestra.exe manual_emitcpp_chain.txt out_manual_emitcpp_chain.cpp.txt --emit=cpp
echo.
type out_manual_emitcpp_chain.cpp.txt

echo.
echo Done.
