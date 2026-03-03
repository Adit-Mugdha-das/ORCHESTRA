#include "bytecode.h"

#include <cstdio>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <algorithm>

#include "symbol_table.h"
#include "interpreter.h"

namespace {

static void vm_fail(FILE* out, const char* msg) {
    if (!out) out = stdout;
    std::fprintf(out, "VM Error: %s\n", msg ? msg : "<unknown>");
    std::fflush(out);
}

enum class VTag : uint8_t { INT, FLOAT, BOOL, STR, STRUCT, NIL };

struct VMValue {
    VTag tag;
    double num;
    bool boolean;
    const char* str;
    void* ptr;
    const char* struct_type;
};

struct VMStruct {
    const char* type_name;
    std::vector<std::string> field_names;
    std::vector<VMValue> field_values;
};

static VMValue v_int(int v) {
    VMValue x{};
    x.tag = VTag::INT;
    x.num = (double)v;
    x.boolean = false;
    x.str = nullptr;
    x.ptr = nullptr;
    x.struct_type = nullptr;
    return x;
}

static VMValue v_float(double v) {
    VMValue x{};
    x.tag = VTag::FLOAT;
    x.num = v;
    x.boolean = false;
    x.str = nullptr;
    x.ptr = nullptr;
    x.struct_type = nullptr;
    return x;
}

static VMValue v_bool(bool b) {
    VMValue x{};
    x.tag = VTag::BOOL;
    x.num = 0;
    x.boolean = b;
    x.str = nullptr;
    x.ptr = nullptr;
    x.struct_type = nullptr;
    return x;
}

static VMValue v_str(const char* s) {
    VMValue x{};
    x.tag = VTag::STR;
    x.num = 0;
    x.boolean = false;
    x.str = s;
    x.ptr = nullptr;
    x.struct_type = nullptr;
    return x;
}

static VMValue v_struct(VMStruct* inst) {
    VMValue x{};
    x.tag = VTag::STRUCT;
    x.num = 0;
    x.boolean = false;
    x.str = nullptr;
    x.ptr = inst;
    x.struct_type = inst ? inst->type_name : nullptr;
    return x;
}

static bool is_numeric(const VMValue& v) {
    return v.tag == VTag::INT || v.tag == VTag::FLOAT;
}

static VMValue v_nil() {
    VMValue x{};
    x.tag = VTag::NIL;
    x.num = 0;
    x.boolean = false;
    x.str = nullptr;
    x.ptr = nullptr;
    x.struct_type = nullptr;
    return x;
}

static VMValue default_for_field_type(const char* t) {
    if (!t) return v_nil();
    if (std::strcmp(t, "int") == 0) return v_int(0);
    if (std::strcmp(t, "float") == 0) return v_float(0);
    if (std::strcmp(t, "bool") == 0) return v_bool(false);
    if (std::strcmp(t, "string") == 0) return v_str("");
    return v_nil();
}

static VMValue coerce_to_field_type(FILE* out, const VMValue& v, const char* field_type) {
    if (!field_type) return v;
    if (std::strcmp(field_type, "int") == 0) {
        if (!is_numeric(v)) {
            vm_fail(out, "Field requires int");
            return v_nil();
        }
        return v_int((int)v.num);
    }
    if (std::strcmp(field_type, "float") == 0) {
        if (!is_numeric(v)) {
            vm_fail(out, "Field requires float");
            return v_nil();
        }
        return v_float(v.num);
    }
    if (std::strcmp(field_type, "bool") == 0) {
        if (v.tag != VTag::BOOL) {
            vm_fail(out, "Field requires bool");
            return v_nil();
        }
        return v;
    }
    if (std::strcmp(field_type, "string") == 0) {
        if (v.tag != VTag::STR) {
            vm_fail(out, "Field requires string");
            return v_nil();
        }
        return v;
    }
    return v;
}

static VMStruct* make_vm_struct(FILE* out, const char* type_name) {
    if (!type_name) {
        vm_fail(out, "Null ensemble type");
        return nullptr;
    }
    if (!is_ensemble_type(type_name)) {
        vm_fail(out, "Unknown ensemble type");
        return nullptr;
    }

    VMStruct* inst = new VMStruct();
    inst->type_name = type_name;

    FieldList* fields = ensemble_fields_or_null(type_name);
    for (FieldList* f = fields; f; f = f->next) {
        inst->field_names.emplace_back(f->name ? f->name : "");
        inst->field_values.push_back(default_for_field_type(f->type));
    }
    return inst;
}

static int find_field_index(VMStruct* inst, const char* field) {
    if (!inst || !field) return -1;
    for (size_t i = 0; i < inst->field_names.size(); i++) {
        if (inst->field_names[i] == field) return (int)i;
    }
    return -1;
}

static int resolve_method_index(const BytecodeProgram& prog, const char* start_type, const char* member) {
    const char* cur_type = start_type;
    while (cur_type && cur_type[0]) {
        std::string cand = std::string(cur_type) + "." + (member ? member : "");
        auto it = prog.func_name_to_index.find(cand);
        if (it != prog.func_name_to_index.end()) return it->second;
        cur_type = ensemble_parent_or_null(cur_type);
    }
    return -1;
}

static int resolve_super_method_index(const BytecodeProgram& prog, const char* child_type, const char* member) {
    const char* parent = ensemble_parent_or_null(child_type);
    if (!parent) return -1;
    return resolve_method_index(prog, parent, member);
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
        case VTag::STRUCT: {
            VMStruct* inst = reinterpret_cast<VMStruct*>(v.ptr);
            const char* tn = (inst && inst->type_name) ? inst->type_name : "";
            std::fprintf(out, "%s{", tn);
            for (size_t i = 0; inst && i < inst->field_names.size(); i++) {
                if (i) std::fprintf(out, ",");
                std::fprintf(out, "%s=", inst->field_names[i].c_str());
                const VMValue fv = inst->field_values[i];
                switch (fv.tag) {
                    case VTag::INT: std::fprintf(out, "%lld", (long long)fv.num); break;
                    case VTag::FLOAT: std::fprintf(out, "%g", fv.num); break;
                    case VTag::BOOL: std::fprintf(out, "%s", fv.boolean ? "true" : "false"); break;
                    case VTag::STR: std::fprintf(out, "%s", fv.str ? fv.str : ""); break;
                    case VTag::STRUCT: {
                        VMStruct* si = reinterpret_cast<VMStruct*>(fv.ptr);
                        const char* st = (si && si->type_name) ? si->type_name : "";
                        std::fprintf(out, "%s{", st);
                        for (size_t j = 0; si && j < si->field_names.size(); j++) {
                            if (j) std::fprintf(out, ",");
                            std::fprintf(out, "%s=", si->field_names[j].c_str());
                            const VMValue sv = si->field_values[j];
                            if (sv.tag == VTag::INT) std::fprintf(out, "%lld", (long long)sv.num);
                            else if (sv.tag == VTag::FLOAT) std::fprintf(out, "%g", sv.num);
                            else if (sv.tag == VTag::BOOL) std::fprintf(out, "%s", sv.boolean ? "true" : "false");
                            else if (sv.tag == VTag::STR) std::fprintf(out, "%s", sv.str ? sv.str : "");
                            else std::fprintf(out, "null");
                        }
                        std::fprintf(out, "}");
                        break;
                    }
                    default:
                        std::fprintf(out, "null");
                        break;
                }
            }
            std::fprintf(out, "}\n");
            break;
        }
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

    if (v.tag == VTag::STRUCT) {
        VMStruct* inst = reinterpret_cast<VMStruct*>(v.ptr);
        declare_or_update_current_scope_struct(name, inst && inst->type_name ? inst->type_name : "", inst);
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

    if (v.tag == VTag::STRUCT) {
        VMStruct* inst = reinterpret_cast<VMStruct*>(v.ptr);
        insert_or_update_struct(name, inst && inst->type_name ? inst->type_name : "", inst);
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

    if (std::strcmp(sym->type, "struct") == 0) {
        VMStruct* inst = reinterpret_cast<VMStruct*>(sym->ptr_value);
        VMValue v = v_struct(inst);
        v.struct_type = sym->struct_type;
        *out_v = v;
        return true;
    }

    vm_fail(out, "Unsupported variable type in Milestone 5");
    return false;
}

static bool vm_execute_impl(FILE* out, const BytecodeProgram& prog, const BytecodeFunc& fn) {
    static const int STACK_MAX = 1024;
    static const int MAX_CALL_DEPTH = 200;

    VMValue stack[STACK_MAX];
    int sp = 0;

    struct Frame {
        const BytecodeFunc* fn;
        size_t ip;
        bool has_this;
        VMValue thisv;
        int post_action; /* 0 none, 1 ctor_result */
        VMValue post_value;
    };
    std::vector<Frame> frames;

    const BytecodeFunc* cur = &fn;
    size_t ip = 0;

    bool has_this = false;
    VMValue thisv = v_nil();

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
            return v_nil();
        }
        return stack[--sp];
    };

    while (ip < cur->code.size()) {
        const Instr ins = cur->code[ip++];
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

            case OpCode::CALL: {
                const int func_index = ins.a;
                const int argc = ins.b;

                if (func_index < 0 || (size_t)func_index >= prog.functions.size()) {
                    vm_fail(out, "Bad function index for CALL");
                    return false;
                }
                if ((int)frames.size() >= MAX_CALL_DEPTH) {
                    vm_fail(out, "Recursion limit exceeded");
                    return false;
                }

                const BytecodeFunc* callee = &prog.functions[(size_t)func_index];
                const int arity = (int)callee->param_name_ids.size();
                if (argc != arity) {
                    vm_fail(out, "Argument count mismatch in CALL");
                    return false;
                }

                /* Save caller frame */
                frames.push_back(Frame{cur, ip, has_this, thisv, 0, v_nil()});

                /* Create param scope (matches interpreter's per-call push_scope) */
                push_scope();

                /* Regular call has no implicit `this`. */
                has_this = false;
                thisv = v_nil();

                /* Bind params in reverse to match stack order */
                for (int i = arity - 1; i >= 0; --i) {
                    VMValue arg = pop();
                    const int pid = callee->param_name_ids[(size_t)i];
                    if (pid < 0 || (size_t)pid >= prog.const_str.size()) {
                        vm_fail(out, "Bad param name id");
                        return false;
                    }
                    store_name_note(out, prog.const_str[(size_t)pid].c_str(), arg);
                }

                /* Transfer control */
                cur = callee;
                ip = 0;
                break;
            }

            case OpCode::CTOR: {
                const int argc = ins.a;
                if ((int)frames.size() >= MAX_CALL_DEPTH) {
                    vm_fail(out, "Recursion limit exceeded");
                    return false;
                }

                std::vector<VMValue> args;
                args.reserve((size_t)argc);
                for (int i = 0; i < argc; i++) args.push_back(pop());
                VMValue typev = pop();
                if (typev.tag != VTag::STR) {
                    vm_fail(out, "CTOR expects type name string");
                    return false;
                }
                std::reverse(args.begin(), args.end());

                VMStruct* inst = make_vm_struct(out, typev.str);
                if (!inst) return false;

                const std::string initName = std::string(typev.str ? typev.str : "") + ".init";
                auto it = prog.func_name_to_index.find(initName);
                if (it != prog.func_name_to_index.end()) {
                    const int fidx = it->second;
                    const BytecodeFunc* callee = &prog.functions[(size_t)fidx];
                    const int arity = (int)callee->param_name_ids.size();
                    if (argc != arity) {
                        vm_fail(out, "Constructor init() argument count mismatch");
                        return false;
                    }

                    /* Save caller frame with post-action: replace init return with struct instance */
                    frames.push_back(Frame{cur, ip, has_this, thisv, 1, v_struct(inst)});
                    push_scope();

                    has_this = true;
                    thisv = v_struct(inst);
                    store_name_note(out, "this", thisv);

                    for (int pi = arity - 1; pi >= 0; --pi) {
                        const int pid = callee->param_name_ids[(size_t)pi];
                        store_name_note(out, prog.const_str[(size_t)pid].c_str(), args[(size_t)pi]);
                    }

                    cur = callee;
                    ip = 0;
                    break;
                }

                /* Field-init fallback: positional args in field order */
                const int fieldc = (int)inst->field_names.size();
                if (argc != fieldc) {
                    if (argc == 0 && fieldc == 0) {
                        if (!push(v_struct(inst))) return false;
                        break;
                    }
                    vm_fail(out, "Constructor argument count mismatch");
                    return false;
                }

                FieldList* fd = ensemble_fields_or_null(typev.str);
                for (int i = 0; i < fieldc; i++) {
                    inst->field_values[(size_t)i] = coerce_to_field_type(out, args[(size_t)i], fd ? fd->type : nullptr);
                    if (fd) fd = fd->next;
                }

                if (!push(v_struct(inst))) return false;
                break;
            }

            case OpCode::FIELD_GET: {
                VMValue fieldv = pop();
                VMValue basev = pop();
                if (basev.tag != VTag::STR || fieldv.tag != VTag::STR) {
                    vm_fail(out, "FIELD_GET expects base/field strings");
                    return false;
                }
                Symbol* sym = get_symbol_or_error(basev.str);
                if (!sym || std::strcmp(sym->type, "struct") != 0) {
                    vm_fail(out, "Field access requires ensemble instance");
                    return false;
                }
                VMStruct* inst = reinterpret_cast<VMStruct*>(sym->ptr_value);
                if (!inst) {
                    vm_fail(out, "Null ensemble instance");
                    return false;
                }
                const int fi = find_field_index(inst, fieldv.str);
                if (fi < 0) {
                    vm_fail(out, "Unknown field on ensemble instance");
                    return false;
                }
                if (!push(inst->field_values[(size_t)fi])) return false;
                break;
            }

            case OpCode::FIELD_SET: {
                VMValue val = pop();
                VMValue fieldv = pop();
                VMValue basev = pop();
                if (basev.tag != VTag::STR || fieldv.tag != VTag::STR) {
                    vm_fail(out, "FIELD_SET expects base/field strings");
                    return false;
                }
                Symbol* sym = get_symbol_or_error(basev.str);
                if (!sym || std::strcmp(sym->type, "struct") != 0) {
                    vm_fail(out, "Field assignment requires ensemble instance");
                    return false;
                }
                VMStruct* inst = reinterpret_cast<VMStruct*>(sym->ptr_value);
                if (!inst) {
                    vm_fail(out, "Null ensemble instance");
                    return false;
                }
                const int fi = find_field_index(inst, fieldv.str);
                if (fi < 0) {
                    vm_fail(out, "Unknown field on ensemble instance");
                    return false;
                }

                FieldList* fd = ensemble_fields_or_null(inst->type_name);
                for (int i = 0; fd && i < fi; i++) fd = fd->next;
                inst->field_values[(size_t)fi] = coerce_to_field_type(out, val, fd ? fd->type : nullptr);
                break;
            }

            case OpCode::DOTCALL: {
                const int argc = ins.a;
                if ((int)frames.size() >= MAX_CALL_DEPTH) {
                    vm_fail(out, "Recursion limit exceeded");
                    return false;
                }

                std::vector<VMValue> args;
                args.reserve((size_t)argc);
                for (int i = 0; i < argc; i++) args.push_back(pop());
                VMValue memberv = pop();
                VMValue basev = pop();
                if (basev.tag != VTag::STR || memberv.tag != VTag::STR) {
                    vm_fail(out, "DOTCALL expects base/member strings");
                    return false;
                }
                std::reverse(args.begin(), args.end());

                Symbol* sym = get_symbol_or_null(basev.str);
                if (sym) {
                    if (std::strcmp(sym->type, "struct") != 0) {
                        vm_fail(out, "Method call requires ensemble/symphony instance");
                        return false;
                    }
                    VMStruct* inst = reinterpret_cast<VMStruct*>(sym->ptr_value);
                    const int fidx = resolve_method_index(prog, inst && inst->type_name ? inst->type_name : "", memberv.str);
                    if (fidx < 0) {
                        vm_fail(out, "Undefined method for type");
                        return false;
                    }
                    const BytecodeFunc* callee = &prog.functions[(size_t)fidx];
                    const int arity = (int)callee->param_name_ids.size();
                    if (argc != arity) {
                        vm_fail(out, "Argument count mismatch in DOTCALL");
                        return false;
                    }

                    frames.push_back(Frame{cur, ip, has_this, thisv, 0, v_nil()});
                    push_scope();
                    has_this = true;
                    thisv = v_struct(inst);
                    store_name_note(out, "this", thisv);
                    for (int pi = arity - 1; pi >= 0; --pi) {
                        const int pid = callee->param_name_ids[(size_t)pi];
                        store_name_note(out, prog.const_str[(size_t)pid].c_str(), args[(size_t)pi]);
                    }
                    cur = callee;
                    ip = 0;
                    break;
                }

                const int fidx = resolve_method_index(prog, basev.str, memberv.str);
                if (fidx < 0) {
                    vm_fail(out, "Call to undefined flow");
                    return false;
                }
                const BytecodeFunc* callee = &prog.functions[(size_t)fidx];
                const int arity = (int)callee->param_name_ids.size();
                if (argc != arity) {
                    vm_fail(out, "Argument count mismatch in DOTCALL");
                    return false;
                }

                frames.push_back(Frame{cur, ip, has_this, thisv, 0, v_nil()});
                push_scope();
                has_this = false;
                thisv = v_nil();
                for (int pi = arity - 1; pi >= 0; --pi) {
                    const int pid = callee->param_name_ids[(size_t)pi];
                    store_name_note(out, prog.const_str[(size_t)pid].c_str(), args[(size_t)pi]);
                }
                cur = callee;
                ip = 0;
                break;
            }

            case OpCode::SUPERCALL: {
                const int argc = ins.a;
                if ((int)frames.size() >= MAX_CALL_DEPTH) {
                    vm_fail(out, "Recursion limit exceeded");
                    return false;
                }

                std::vector<VMValue> args;
                args.reserve((size_t)argc);
                for (int i = 0; i < argc; i++) args.push_back(pop());
                VMValue memberv = pop();
                if (memberv.tag != VTag::STR) {
                    vm_fail(out, "SUPERCALL expects member string");
                    return false;
                }
                std::reverse(args.begin(), args.end());

                if (!has_this || thisv.tag != VTag::STRUCT) {
                    vm_fail(out, "super.method() requires a method context with this");
                    return false;
                }
                VMStruct* inst = reinterpret_cast<VMStruct*>(thisv.ptr);
                const int fidx = resolve_super_method_index(prog, inst && inst->type_name ? inst->type_name : "", memberv.str);
                if (fidx < 0) {
                    vm_fail(out, "Undefined super method");
                    return false;
                }
                const BytecodeFunc* callee = &prog.functions[(size_t)fidx];
                const int arity = (int)callee->param_name_ids.size();
                if (argc != arity) {
                    vm_fail(out, "Argument count mismatch in SUPERCALL");
                    return false;
                }

                frames.push_back(Frame{cur, ip, has_this, thisv, 0, v_nil()});
                push_scope();
                /* keep same this */
                store_name_note(out, "this", thisv);
                for (int pi = arity - 1; pi >= 0; --pi) {
                    const int pid = callee->param_name_ids[(size_t)pi];
                    store_name_note(out, prog.const_str[(size_t)pid].c_str(), args[(size_t)pi]);
                }
                cur = callee;
                ip = 0;
                break;
            }

            case OpCode::SUPERCTOR: {
                const int argc = ins.a;
                if ((int)frames.size() >= MAX_CALL_DEPTH) {
                    vm_fail(out, "Recursion limit exceeded");
                    return false;
                }

                std::vector<VMValue> args;
                args.reserve((size_t)argc);
                for (int i = 0; i < argc; i++) args.push_back(pop());
                std::reverse(args.begin(), args.end());

                if (!has_this || thisv.tag != VTag::STRUCT) {
                    vm_fail(out, "super() requires a method context with this");
                    return false;
                }
                VMStruct* inst = reinterpret_cast<VMStruct*>(thisv.ptr);
                const char* parent = ensemble_parent_or_null(inst && inst->type_name ? inst->type_name : "");
                if (!parent) {
                    vm_fail(out, "super() used but type has no parent");
                    return false;
                }

                const std::string initName = std::string(parent) + ".init";
                auto it = prog.func_name_to_index.find(initName);
                if (it != prog.func_name_to_index.end()) {
                    const int fidx = it->second;
                    const BytecodeFunc* callee = &prog.functions[(size_t)fidx];
                    const int arity = (int)callee->param_name_ids.size();
                    if (argc != arity) {
                        vm_fail(out, "super() argument count mismatch");
                        return false;
                    }

                    frames.push_back(Frame{cur, ip, has_this, thisv, 0, v_nil()});
                    push_scope();
                    store_name_note(out, "this", thisv);
                    for (int pi = arity - 1; pi >= 0; --pi) {
                        const int pid = callee->param_name_ids[(size_t)pi];
                        store_name_note(out, prog.const_str[(size_t)pid].c_str(), args[(size_t)pi]);
                    }
                    cur = callee;
                    ip = 0;
                    break;
                }

                /* Fallback: initialize parent fields directly (prefix). */
                FieldList* pfields = ensemble_fields_or_null(parent);
                int fieldc = 0;
                for (FieldList* f = pfields; f; f = f->next) fieldc++;
                if (argc != fieldc) {
                    vm_fail(out, "super() argument count mismatch");
                    return false;
                }
                for (int i = 0; i < fieldc; i++) {
                    inst->field_values[(size_t)i] = coerce_to_field_type(out, args[(size_t)i], pfields ? pfields->type : nullptr);
                    if (pfields) pfields = pfields->next;
                }
                if (!push(v_int(0))) return false;
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
                if (ins.a < 0 || (size_t)ins.a > cur->code.size()) {
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
                    if (ins.a < 0 || (size_t)ins.a > cur->code.size()) {
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

            case OpCode::RET: {
                if (frames.empty()) {
                    return true;
                }

                /* Pop per-call param scope */
                pop_scope();

                Frame fr = frames.back();
                frames.pop_back();
                cur = fr.fn;
                ip = fr.ip;

                /* Apply optional post-return action (used by CTOR calling init()). */
                if (fr.post_action == 1) {
                    (void)pop(); /* discard callee return */
                    if (!push(fr.post_value)) return false;
                }

                has_this = fr.has_this;
                thisv = fr.thisv;
                break;
            }

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
        case OpCode::CALL: return "CALL";
        case OpCode::CTOR: return "CTOR";
        case OpCode::FIELD_GET: return "FIELD_GET";
        case OpCode::FIELD_SET: return "FIELD_SET";
        case OpCode::DOTCALL: return "DOTCALL";
        case OpCode::SUPERCALL: return "SUPERCALL";
        case OpCode::SUPERCTOR: return "SUPERCTOR";
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
