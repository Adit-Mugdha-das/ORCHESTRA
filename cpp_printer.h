#ifndef CPP_PRINTER_H
#define CPP_PRINTER_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "interpreter.h"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
  ORCHESTRA AST → C++-style pretty printer.
  Learning/debug only: output is intended to be readable, not necessarily compilable.
*/

typedef enum OrchCppEmitStyle {
  ORCH_CPP_STYLE_CPP = 0,
  ORCH_CPP_STYLE_PSEUDO = 1,
} OrchCppEmitStyle;

void dump_cpp_program_with_style(Stmt* mainBlock, FILE* out, OrchCppEmitStyle style);
void dump_cpp_program(Stmt* mainBlock, FILE* out);

#ifdef __cplusplus
}
#endif

#endif
