#ifndef BYTECODE_H
#define BYTECODE_H

#include <cstdio>
#include <deque>
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

    /* Variables + scopes (Milestone 5, name-based) */
    SCOPE_PUSH,
    SCOPE_POP,
    LOAD_NAME,  /* a = const_str index */
    NOTE_NAME,  /* a = const_str index; pops 1 value */
    STAGE_NAME, /* a = const_str index; pops 1 value */

    /* Calls (Milestone 7) */
    CALL, /* a = func index, b = argc */

    /* Arithmetic */
    ADD,
    SUB,
    MUL,
    DIV,
    NEG,
    NOT,

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

    /* For calls (Milestone 7): parameter names for this function. */
    std::vector<int> param_name_ids; /* const_str indices */

    void emit(OpCode op, int a = 0, int b = 0) {
        code.push_back(Instr{op, a, b});
    }
};

struct BytecodeProgram {
    /* Constant pools (Milestone 2) */
    std::vector<double> const_num;
    std::vector<std::string> const_str;

    std::deque<BytecodeFunc> functions;

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

/* Execute a single bytecode function (Milestone 2+). */
bool vm_execute(FILE* out, const BytecodeProgram& prog, const BytecodeFunc& fn);

#endif
