#include "interpreter.h"

#include "bytecode.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

static FILE* g_compile_out = nullptr;

static void vm_compile_error(FILE* out, const char* msg) {
    if (!out) out = stdout;
    std::fprintf(out, "VM Compile Error: %s\n", msg ? msg : "<unknown>");
    std::fflush(out);
}

static void compile_expr(BytecodeProgram& prog, BytecodeFunc& fn, Expr* e);

static void compile_lit(BytecodeProgram& prog, BytecodeFunc& fn, const Expr* e) {
    if (std::strcmp(e->lit_type, "int") == 0) {
        fn.emit(OpCode::PUSH_INT, (int)e->lit_num);
        return;
    }
    if (std::strcmp(e->lit_type, "float") == 0) {
        const int idx = prog.add_num(e->lit_num);
        fn.emit(OpCode::PUSH_FLOAT, idx);
        return;
    }
    if (std::strcmp(e->lit_type, "bool") == 0) {
        fn.emit(OpCode::PUSH_BOOL, e->lit_bool ? 1 : 0);
        return;
    }
    if (std::strcmp(e->lit_type, "string") == 0) {
        const int idx = prog.add_str(e->lit_str);
        fn.emit(OpCode::PUSH_STR, idx);
        return;
    }

    vm_compile_error(g_compile_out, "Unsupported literal type");
    std::exit(1);
}

static void compile_unary(BytecodeProgram& prog, BytecodeFunc& fn, Expr* e) {
    compile_expr(prog, fn, e->left);

    if (e->op == OP_NEG) {
        fn.emit(OpCode::NEG);
        return;
    }
    if (e->op == OP_NOT) {
        fn.emit(OpCode::NOT);
        return;
    }

    vm_compile_error(g_compile_out, "Unsupported unary operator");
    std::exit(1);
}

static void compile_binary(BytecodeProgram& prog, BytecodeFunc& fn, Expr* e) {
    compile_expr(prog, fn, e->left);
    compile_expr(prog, fn, e->right);

    switch (e->op) {
        case OP_PLUS: fn.emit(OpCode::ADD); return;
        case OP_MINUS: fn.emit(OpCode::SUB); return;
        case OP_MUL: fn.emit(OpCode::MUL); return;
        case OP_DIV: fn.emit(OpCode::DIV); return;

        case OP_EQ: fn.emit(OpCode::EQ); return;
        case OP_NE: fn.emit(OpCode::NE); return;
        case OP_LT: fn.emit(OpCode::LT); return;
        case OP_LE: fn.emit(OpCode::LE); return;
        case OP_GT: fn.emit(OpCode::GT); return;
        case OP_GE: fn.emit(OpCode::GE); return;

        default:
            vm_compile_error(g_compile_out, "Unsupported binary operator");
            std::exit(1);
    }
}

static void compile_expr(BytecodeProgram& prog, BytecodeFunc& fn, Expr* e) {
    if (!e) {
        vm_compile_error(g_compile_out, "Null expression");
        std::exit(1);
    }

    switch (e->kind) {
        case EXPR_LIT:
            compile_lit(prog, fn, e);
            return;
        case EXPR_UNARY:
            compile_unary(prog, fn, e);
            return;
        case EXPR_BIN:
            compile_binary(prog, fn, e);
            return;
        case EXPR_CHAIN: {
            /* Grammar encodes even a single comparison as EXPR_CHAIN.
               Milestone 3 supports only the 2-operand form: a < b, a == b, ... */
            if (!e->chain_rest || e->chain_rest->next) {
                vm_compile_error(g_compile_out, "Chained comparisons not supported yet");
                std::exit(1);
            }

            compile_expr(prog, fn, e->chain_first);
            compile_expr(prog, fn, e->chain_rest->expr);

            switch (e->chain_rest->op) {
                case OP_EQ: fn.emit(OpCode::EQ); break;
                case OP_NE: fn.emit(OpCode::NE); break;
                case OP_LT: fn.emit(OpCode::LT); break;
                case OP_LE: fn.emit(OpCode::LE); break;
                case OP_GT: fn.emit(OpCode::GT); break;
                case OP_GE: fn.emit(OpCode::GE); break;
                default:
                    vm_compile_error(g_compile_out, "Unsupported comparison operator");
                    std::exit(1);
            }
            return;
        }
        default:
            {
                char buf[128];
                std::snprintf(buf, sizeof(buf), "Expression kind %d not supported in Milestone 3", e->kind);
                vm_compile_error(g_compile_out, buf);
            }
            std::exit(1);
    }
}

static void compile_stmt(BytecodeProgram& prog, BytecodeFunc& fn, Stmt* s);

static void compile_stmt_list(BytecodeProgram& prog, BytecodeFunc& fn, StmtList* list) {
    for (StmtList* cur = list; cur; cur = cur->next) {
        compile_stmt(prog, fn, cur->stmt);
    }
}

static void compile_stmt(BytecodeProgram& prog, BytecodeFunc& fn, Stmt* s) {
    if (!s) return;

    switch (s->kind) {
        case STMT_EMIT:
            compile_expr(prog, fn, s->expr);
            fn.emit(OpCode::EMIT);
            return;
        case STMT_BLOCK:
            compile_stmt_list(prog, fn, s->list);
            return;
        default:
            vm_compile_error(g_compile_out, "Statement kind not supported yet (Milestone 3/4)");
            std::exit(1);
    }
}

extern "C" int execute_program_vm(Stmt* root, FILE* out) {
    if (!out) out = stdout;
    g_compile_out = out;
    if (!root) {
        std::fprintf(out, "VM backend: no entry block\n");
        return 1;
    }

    BytecodeProgram prog;
    prog.functions.emplace_back();
    BytecodeFunc& fn = prog.functions.back();

    compile_stmt(prog, fn, root);
    fn.emit(OpCode::RET);

    const bool ok = vm_execute(out, prog, fn);
    return ok ? 0 : 1;
}
