#ifndef BYTECODE_H
#define BYTECODE_H

#include <string>
#include <vector>

/* Minimal stack-bytecode IR (Milestone 1).
   This header is C++-only. C code should interact via bytecode_unit_smoke(). */

enum class OpCode {
    NOP = 0,

    /* Stack */
    PUSH_INT,
    PUSH_FLOAT, /* a = const_num index */
    PUSH_BOOL,
    PUSH_STR,   /* a = const_str index */
    POP,

    /* Arithmetic */
    ADD,
    SUB,
    MUL,
    DIV,
    NEG,

    /* Comparison */
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE,

    /* Output */
    EMIT,

    /* Control flow */
    JMP,          /* a = target ip */
    JMP_IF_FALSE, /* a = target ip; pops 1 bool */

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
    /* Constant pools (Milestone 2) */
    std::vector<double> const_num;
    std::vector<std::string> const_str;

    std::vector<BytecodeFunc> functions;

    int add_num(double v) {
        const_num.push_back(v);
        return (int)const_num.size() - 1;
    }
    int add_str(const std::string& s) {
        const_str.push_back(s);
        return (int)const_str.size() - 1;
    }
};

const char* opcode_name(OpCode op);
void dump_bytecode_func(const BytecodeFunc& fn, void* c_file /* FILE* */);

#endif
