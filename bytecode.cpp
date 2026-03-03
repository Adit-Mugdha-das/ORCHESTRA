#include "bytecode.h"

#include <cstdio>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "symbol_table.h"

namespace {

static void vm_fail(FILE* out, const char* msg) {
    if (!out) out = stdout;
    std::fprintf(out, "VM Error: %s\n", msg ? msg : "<unknown>");
    std::fflush(out);
}

enum class VTag : uint8_t { INT, FLOAT, BOOL, STR, NIL };

struct VMValue {
    VTag tag;
    double num;
    bool boolean;
    const char* str;
};

static VMValue v_int(int v) {
    VMValue x{};
    x.tag = VTag::INT;
    x.num = (double)v;
    x.boolean = false;
    x.str = nullptr;
    return x;
}

static VMValue v_float(double v) {
    VMValue x{};
    x.tag = VTag::FLOAT;
    x.num = v;
    x.boolean = false;
    x.str = nullptr;
    return x;
}

static VMValue v_bool(bool b) {
    VMValue x{};
    x.tag = VTag::BOOL;
    x.num = 0;
    x.boolean = b;
    x.str = nullptr;
    return x;
}

static VMValue v_str(const char* s) {
    VMValue x{};
    x.tag = VTag::STR;
    x.num = 0;
    x.boolean = false;
    x.str = s;
    return x;
}

static bool is_numeric(const VMValue& v) {
    return v.tag == VTag::INT || v.tag == VTag::FLOAT;
}

static void print_value(FILE* out, const VMValue& v) {
    if (!out) out = stdout;
    switch (v.tag) {
        case VTag::INT:
            std::fprintf(out, "%lld\n", (long long)v.num);
            break;
        case VTag::FLOAT:
            std::fprintf(out, "%g\n", v.num);
            break;
        case VTag::BOOL:
            std::fprintf(out, "%s\n", v.boolean ? "true" : "false");
            break;
        case VTag::STR:
            std::fprintf(out, "%s\n", v.str ? v.str : "");
            break;
        default:
            std::fprintf(out, "null\n");
            break;
    }
}

static void store_name_note(FILE* out, const char* name, const VMValue& v) {
    if (!name) {
        vm_fail(out, "Null variable name");
        return;
    }
    if (v.tag == VTag::INT) {
        declare_or_update_current_scope_value(name, "int", v.num, "", 0);
        return;
    }
    if (v.tag == VTag::FLOAT) {
        declare_or_update_current_scope_value(name, "float", v.num, "", 0);
        return;
    }
    if (v.tag == VTag::BOOL) {
        declare_or_update_current_scope_value(name, "bool", 0, "", v.boolean ? 1 : 0);
        return;
    }
    if (v.tag == VTag::STR) {
        declare_or_update_current_scope_value(name, "string", 0, v.str ? v.str : "", 0);
        return;
    }

    vm_fail(out, "Unsupported value type for NOTE");
}

static void store_name_stage(FILE* out, const char* name, const VMValue& v) {
    if (!name) {
        vm_fail(out, "Null variable name");
        return;
    }
    if (v.tag == VTag::INT) {
        insert_or_update_value(name, "int", v.num, "", 0);
        return;
    }
    if (v.tag == VTag::FLOAT) {
        insert_or_update_value(name, "float", v.num, "", 0);
        return;
    }
    if (v.tag == VTag::BOOL) {
        insert_or_update_value(name, "bool", 0, "", v.boolean ? 1 : 0);
        return;
    }
    if (v.tag == VTag::STR) {
        insert_or_update_value(name, "string", 0, v.str ? v.str : "", 0);
        return;
    }

    vm_fail(out, "Unsupported value type for STAGE");
}

static bool load_name(FILE* out, const char* name, VMValue* out_v) {
    if (!out_v) return false;
    if (!name) {
        vm_fail(out, "Null variable name");
        return false;
    }

    Symbol* sym = get_symbol_or_error(name);
    if (!sym) {
        vm_fail(out, "Undefined variable");
        return false;
    }

    if (std::strcmp(sym->type, "int") == 0) {
        *out_v = v_int((int)sym->num_value);
        return true;
    }
    if (std::strcmp(sym->type, "float") == 0) {
        *out_v = v_float(sym->num_value);
        return true;
    }
    if (std::strcmp(sym->type, "bool") == 0) {
        *out_v = v_bool(sym->bool_value != 0);
        return true;
    }
    if (std::strcmp(sym->type, "string") == 0) {
        *out_v = v_str(sym->str_value);
        return true;
    }

    vm_fail(out, "Unsupported variable type in Milestone 5");
    return false;
}

static bool vm_execute_impl(FILE* out, const BytecodeProgram& prog, const BytecodeFunc& fn) {
    static const int STACK_MAX = 1024;
    VMValue stack[STACK_MAX];
    int sp = 0;

    auto push = [&](const VMValue& v) -> bool {
        if (sp >= STACK_MAX) {
            vm_fail(out, "Stack overflow");
            return false;
        }
        stack[sp++] = v;
        return true;
    };

    auto pop = [&]() -> VMValue {
        if (sp <= 0) {
            vm_fail(out, "Stack underflow");
            return VMValue{VTag::NIL, 0, false, nullptr};
        }
        return stack[--sp];
    };

    size_t ip = 0;
    while (ip < fn.code.size()) {
        const Instr ins = fn.code[ip++];
        switch (ins.op) {
            case OpCode::NOP:
                break;

            case OpCode::PUSH_INT:
                if (!push(v_int(ins.a))) return false;
                break;
            case OpCode::PUSH_BOOL:
                if (!push(v_bool(ins.a != 0))) return false;
                break;
            case OpCode::PUSH_FLOAT: {
                if (ins.a < 0 || (size_t)ins.a >= prog.const_num.size()) {
                    vm_fail(out, "Bad const_num index");
                    return false;
                }
                if (!push(v_float(prog.const_num[(size_t)ins.a]))) return false;
                break;
            }
            case OpCode::PUSH_STR: {
                if (ins.a < 0 || (size_t)ins.a >= prog.const_str.size()) {
                    vm_fail(out, "Bad const_str index");
                    return false;
                }
                if (!push(v_str(prog.const_str[(size_t)ins.a].c_str()))) return false;
                break;
            }
            case OpCode::POP:
                (void)pop();
                break;

            case OpCode::SCOPE_PUSH:
                push_scope();
                break;
            case OpCode::SCOPE_POP:
                pop_scope();
                break;
            case OpCode::LOAD_NAME: {
                if (ins.a < 0 || (size_t)ins.a >= prog.const_str.size()) {
                    vm_fail(out, "Bad const_str index for LOAD_NAME");
                    return false;
                }
                VMValue v;
                if (!load_name(out, prog.const_str[(size_t)ins.a].c_str(), &v)) return false;
                if (!push(v)) return false;
                break;
            }
            case OpCode::NOTE_NAME: {
                if (ins.a < 0 || (size_t)ins.a >= prog.const_str.size()) {
                    vm_fail(out, "Bad const_str index for NOTE_NAME");
                    return false;
                }
                VMValue v = pop();
                store_name_note(out, prog.const_str[(size_t)ins.a].c_str(), v);
                break;
            }
            case OpCode::STAGE_NAME: {
                if (ins.a < 0 || (size_t)ins.a >= prog.const_str.size()) {
                    vm_fail(out, "Bad const_str index for STAGE_NAME");
                    return false;
                }
                VMValue v = pop();
                store_name_stage(out, prog.const_str[(size_t)ins.a].c_str(), v);
                break;
            }

            case OpCode::NEG: {
                VMValue a = pop();
                if (!is_numeric(a)) {
                    vm_fail(out, "NEG expects numeric");
                    return false;
                }
                if (a.tag == VTag::INT) {
                    if (!push(v_int(-(int)a.num))) return false;
                } else {
                    if (!push(v_float(-a.num))) return false;
                }
                break;
            }

            case OpCode::NOT: {
                VMValue a = pop();
                if (a.tag != VTag::BOOL) {
                    vm_fail(out, "NOT expects bool");
                    return false;
                }
                if (!push(v_bool(!a.boolean))) return false;
                break;
            }

            case OpCode::ADD:
            case OpCode::SUB:
            case OpCode::MUL:
            case OpCode::DIV: {
                VMValue b = pop();
                VMValue a = pop();
                if (!is_numeric(a) || !is_numeric(b)) {
                    vm_fail(out, "Arithmetic expects numeric");
                    return false;
                }
                const bool any_float = (a.tag == VTag::FLOAT) || (b.tag == VTag::FLOAT) || (ins.op == OpCode::DIV);
                double r = 0.0;
                if (ins.op == OpCode::ADD) r = a.num + b.num;
                else if (ins.op == OpCode::SUB) r = a.num - b.num;
                else if (ins.op == OpCode::MUL) r = a.num * b.num;
                else {
                    if (b.num == 0.0) {
                        vm_fail(out, "Division by zero");
                        return false;
                    }
                    r = a.num / b.num;
                }

                if (!any_float) {
                    if (!push(v_int((int)r))) return false;
                } else {
                    if (!push(v_float(r))) return false;
                }
                break;
            }

            case OpCode::EQ:
            case OpCode::NE:
            case OpCode::LT:
            case OpCode::LE:
            case OpCode::GT:
            case OpCode::GE: {
                VMValue b = pop();
                VMValue a = pop();

                bool res = false;
                if (is_numeric(a) && is_numeric(b)) {
                    const double x = a.num;
                    const double y = b.num;
                    switch (ins.op) {
                        case OpCode::EQ: res = (x == y); break;
                        case OpCode::NE: res = (x != y); break;
                        case OpCode::LT: res = (x < y); break;
                        case OpCode::LE: res = (x <= y); break;
                        case OpCode::GT: res = (x > y); break;
                        case OpCode::GE: res = (x >= y); break;
                        default: break;
                    }
                } else if (a.tag == VTag::BOOL && b.tag == VTag::BOOL) {
                    switch (ins.op) {
                        case OpCode::EQ: res = (a.boolean == b.boolean); break;
                        case OpCode::NE: res = (a.boolean != b.boolean); break;
                        default:
                            vm_fail(out, "Ordering compares require numeric");
                            return false;
                    }
                } else if (a.tag == VTag::STR && b.tag == VTag::STR) {
                    const int c = std::strcmp(a.str ? a.str : "", b.str ? b.str : "");
                    switch (ins.op) {
                        case OpCode::EQ: res = (c == 0); break;
                        case OpCode::NE: res = (c != 0); break;
                        case OpCode::LT: res = (c < 0); break;
                        case OpCode::LE: res = (c <= 0); break;
                        case OpCode::GT: res = (c > 0); break;
                        case OpCode::GE: res = (c >= 0); break;
                        default: break;
                    }
                } else {
                    /* Match interpreter: ==/!= operands must be comparable (same-kind). */
                    vm_fail(out, "==/!= operands must be comparable");
                    return false;
                }

                if (!push(v_bool(res))) return false;
                break;
            }

            case OpCode::JMP:
                if (ins.a < 0 || (size_t)ins.a > fn.code.size()) {
                    vm_fail(out, "Bad jump target");
                    return false;
                }
                ip = (size_t)ins.a;
                break;

            case OpCode::JMP_IF_FALSE: {
                VMValue cond = pop();
                if (cond.tag != VTag::BOOL) {
                    vm_fail(out, "JMP_IF_FALSE expects bool");
                    return false;
                }
                if (!cond.boolean) {
                    if (ins.a < 0 || (size_t)ins.a > fn.code.size()) {
                        vm_fail(out, "Bad jump target");
                        return false;
                    }
                    ip = (size_t)ins.a;
                }
                break;
            }

            case OpCode::EMIT: {
                VMValue v = pop();
                print_value(out, v);
                break;
            }

            case OpCode::RET:
                return true;

            default:
                vm_fail(out, "Unknown opcode");
                return false;
        }
    }

    return true;
}

} // namespace

bool vm_execute(FILE* out, const BytecodeProgram& prog, const BytecodeFunc& fn) {
    return vm_execute_impl(out, prog, fn);
}

static FILE* as_file(void* f) {
    return reinterpret_cast<FILE*>(f);
}

const char* opcode_name(OpCode op) {
    switch (op) {
        case OpCode::NOP: return "NOP";
        case OpCode::PUSH_INT: return "PUSH_INT";
        case OpCode::PUSH_FLOAT: return "PUSH_FLOAT";
        case OpCode::PUSH_BOOL: return "PUSH_BOOL";
        case OpCode::PUSH_STR: return "PUSH_STR";
        case OpCode::POP: return "POP";
        case OpCode::SCOPE_PUSH: return "SCOPE_PUSH";
        case OpCode::SCOPE_POP: return "SCOPE_POP";
        case OpCode::LOAD_NAME: return "LOAD_NAME";
        case OpCode::NOTE_NAME: return "NOTE_NAME";
        case OpCode::STAGE_NAME: return "STAGE_NAME";
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::MUL: return "MUL";
        case OpCode::DIV: return "DIV";
        case OpCode::NEG: return "NEG";
        case OpCode::NOT: return "NOT";
        case OpCode::EQ: return "EQ";
        case OpCode::NE: return "NE";
        case OpCode::LT: return "LT";
        case OpCode::LE: return "LE";
        case OpCode::GT: return "GT";
        case OpCode::GE: return "GE";
        case OpCode::EMIT: return "EMIT";
        case OpCode::JMP: return "JMP";
        case OpCode::JMP_IF_FALSE: return "JMP_IF_FALSE";
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

    (void)vm_execute(out, prog, f);
}
