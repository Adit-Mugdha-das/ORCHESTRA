#ifndef BYTECODE_H
#define BYTECODE_H

#include <cstdio>
#include <deque>
#include <unordered_map>
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

    /* OOP / ensembles (Milestone 8) */
    CTOR,      /* a = argc; stack: type_str, args... -> pushes struct */
    FIELD_GET, /* stack: base_str, field_str -> pushes value */
    FIELD_SET, /* stack: base_str, field_str, value -> (sets field) */
    DOTCALL,   /* a = argc; stack: base_str, member_str, args... -> pushes return */
    SUPERCALL, /* a = argc; stack: member_str, args... -> pushes return */
    SUPERCTOR, /* a = argc; stack: args... -> pushes return */

    /* Collections (Milestone 9) */
    ARRAY_LIT,   /* a = count; stack: elems... -> pushes array */
    INDEX_GET,   /* stack: target, index -> pushes value */
    INDEX_SET,   /* stack: target, index, value -> (sets) */

    ARRAY_NEW,    /* stack: n -> pushes array */
    ARRAY_PUSH,   /* stack: array, value -> pushes new length(int) */
    ARRAY_POP,    /* stack: array -> pushes popped value */
    ARRAY_RESIZE, /* stack: array, newSize -> pushes length(int) */

    MAP_NEW, /* pushes map */
    SET_NEW, /* pushes set */

    SET_ADD,    /* stack: set, key -> pushes size(int) */
    COLL_HAS,   /* stack: (map|set), key -> pushes bool */
    MAP_GET,    /* stack: map, key -> pushes value */
    MAP_PUT,    /* stack: map, key, value -> pushes value */
    COLL_DEL,   /* stack: (map|set), key -> pushes bool */
    COLL_KEYS,  /* stack: (map|set) -> pushes array of string */

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
    PLAY,         /* a = argc; pushes args then format_str in order; formatted print, no newline */

    /* Pointers */
    ADDROF,       /* a = const_str index of variable name; pushes PTR value */
    DEREF_OP,     /* stack: ptr -> pushes the referenced variable's value */
    STORE_THRU,   /* stack: ptr, value -> stores value into the variable ptr points at */

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

    /* Runtime resolution support for Milestone 8 (dotcall/super). */
    std::unordered_map<std::string, int> func_name_to_index;

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
