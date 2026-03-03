#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>

/* Minimal stack-bytecode IR (Milestone 1).
   This header is C++-only. C code should interact via bytecode_unit_smoke(). */

enum class OpCode {
    NOP = 0,

    /* Stack */
    PUSH_INT,
    PUSH_BOOL,
    POP,

    /* Arithmetic */
    ADD,
    MUL,

    /* Output */
    EMIT,

    /* Control */
    RET,
};

struct Instr {
    OpCode op;
    int a;
    int b;
};

struct BytecodeFunc {
    std::vector<Instr> code;

    void emit(OpCode op, int a = 0, int b = 0) {
        code.push_back(Instr{op, a, b});
    }
};

struct BytecodeProgram {
    /* Placeholder for future:
       - functions by name
       - constant pools
       - debug tables
    */
    std::vector<BytecodeFunc> functions;
};

const char* opcode_name(OpCode op);
void dump_bytecode_func(const BytecodeFunc& fn, void* c_file /* FILE* */);

#endif
