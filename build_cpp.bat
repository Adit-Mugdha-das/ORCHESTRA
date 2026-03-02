@echo off
setlocal
cd /d "%~dp0"

rem Regenerate parser/lexer
bison -d orchestra.y || exit /b 1
flex orchestra.l || exit /b 1

rem Compile C parts
gcc -c orchestra.tab.c -o orchestra.tab.o || exit /b 1
gcc -c lex.yy.c -o lex.yy.o || exit /b 1

rem Compile interpreter as C++ (Option B)
g++ -c interpreter.cpp -o interpreter.o || exit /b 1

rem Compile symbol table as C++
g++ -c symbol_table.cpp -o symbol_table.o || exit /b 1

rem Compile flow registry as C++
g++ -c flow_registry.cpp -o flow_registry.o || exit /b 1

rem Link with C++ linker

g++ orchestra.tab.o lex.yy.o interpreter.o symbol_table.o flow_registry.o -o orchestra.exe || exit /b 1

echo Built orchestra.exe (C parser/lexer + C++-compiled interpreter)
