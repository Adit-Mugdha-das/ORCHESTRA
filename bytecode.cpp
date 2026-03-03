#include "bytecode.h"

#include <cstdio>

static FILE* as_file(void* f) {
    return reinterpret_cast<FILE*>(f);
}

const char* opcode_name(OpCode op) {
    switch (op) {
        case OpCode::NOP: return "NOP";
        case OpCode::PUSH_INT: return "PUSH_INT";
        case OpCode::PUSH_BOOL: return "PUSH_BOOL";
        case OpCode::POP: return "POP";
        case OpCode::ADD: return "ADD";
        case OpCode::MUL: return "MUL";
        case OpCode::EMIT: return "EMIT";
        case OpCode::RET: return "RET";
        default: return "<UNKNOWN_OP>";
    }
}

void dump_bytecode_func(const BytecodeFunc& fn, void* c_file) {
    FILE* out = as_file(c_file);
    if (!out) out = stdout;

    for (size_t i = 0; i < fn.code.size(); i++) {
        const Instr& ins = fn.code[i];
        std::fprintf(out, "%zu: %s %d %d\n", i, opcode_name(ins.op), ins.a, ins.b);
    }
}

/* C-callable unit smoke (Milestone 1 verification).
   This lets the C-built parser/main invoke the bytecode module without including C++ headers.
*/
extern "C" void bytecode_unit_smoke(void* c_file /* FILE* */) {
    FILE* out = as_file(c_file);
    if (!out) out = stdout;

    std::fprintf(out, "[VM] Bytecode IR smoke dump (Milestone 1)\n");

    BytecodeProgram prog;
    prog.functions.emplace_back();

    BytecodeFunc& f = prog.functions.back();

    /* Represents: emit 1 + 2 * 3; */
    f.emit(OpCode::PUSH_INT, 1);
    f.emit(OpCode::PUSH_INT, 2);
    f.emit(OpCode::PUSH_INT, 3);
    f.emit(OpCode::MUL);
    f.emit(OpCode::ADD);
    f.emit(OpCode::EMIT);
    f.emit(OpCode::RET);

    dump_bytecode_func(f, out);
}
