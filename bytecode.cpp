#include "bytecode.h"

#include <cstdio>
#include <cmath>
#include <cstdint>
#include <cstring>

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

static bool vm_execute(FILE* out, const BytecodeProgram& prog, const BytecodeFunc& fn) {
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
                    /* Keep ints as int when division is exact and both were ints. */
                    if (ins.op == OpCode::DIV && a.tag == VTag::INT && b.tag == VTag::INT) {
                        double ir = std::round(r);
                        if (std::fabs(r - ir) < 1e-12) {
                            if (!push(v_int((int)ir))) return false;
                            break;
                        }
                    }
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
                    /* Different types: only allow EQ/NE (false/true). */
                    if (ins.op == OpCode::EQ) res = false;
                    else if (ins.op == OpCode::NE) res = true;
                    else {
                        vm_fail(out, "Ordering compares require same-type numeric");
                        return false;
                    }
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
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::MUL: return "MUL";
        case OpCode::DIV: return "DIV";
        case OpCode::NEG: return "NEG";
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
