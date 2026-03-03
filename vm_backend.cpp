#include "interpreter.h"

#include "bytecode.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static FILE* g_compile_out = nullptr;

static void vm_compile_error(FILE* out, const char* msg) {
    if (!out) out = stdout;
    std::fprintf(out, "VM Compile Error: %s\n", msg ? msg : "<unknown>");
    std::fflush(out);
}

static void compile_expr(BytecodeProgram& prog, BytecodeFunc& fn, Expr* e);
static void compile_stmt(BytecodeProgram& prog, BytecodeFunc& fn, Stmt* s);

struct LoopContext {
    size_t continue_target;
    std::vector<size_t> break_jumps;
};

static std::vector<LoopContext> g_loops;

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
        case EXPR_VAR: {
            if (!e->name) {
                vm_compile_error(g_compile_out, "Null variable name");
                std::exit(1);
            }
            const int id = prog.add_str(e->name);
            fn.emit(OpCode::LOAD_NAME, id);
            return;
        }
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

static void compile_stmt_list(BytecodeProgram& prog, BytecodeFunc& fn, StmtList* list) {
    for (StmtList* cur = list; cur; cur = cur->next) {
        compile_stmt(prog, fn, cur->stmt);
    }
}

static size_t emit_jump(BytecodeFunc& fn, OpCode op, int target_placeholder = 0) {
    const size_t at = fn.code.size();
    fn.emit(op, target_placeholder);
    return at;
}

static void patch_jump(BytecodeFunc& fn, size_t at, size_t target) {
    if (at >= fn.code.size()) return;
    fn.code[at].a = (int)target;
}

static void compile_stmt(BytecodeProgram& prog, BytecodeFunc& fn, Stmt* s) {
    if (!s) return;

    switch (s->kind) {
        case STMT_EMIT:
            compile_expr(prog, fn, s->expr);
            fn.emit(OpCode::EMIT);
            return;
        case STMT_BLOCK:
            fn.emit(OpCode::SCOPE_PUSH);
            compile_stmt_list(prog, fn, s->list);
            fn.emit(OpCode::SCOPE_POP);
            return;
        case STMT_NOTE: {
            if (!s->name) {
                vm_compile_error(g_compile_out, "Null NOTE name");
                std::exit(1);
            }
            compile_expr(prog, fn, s->expr);
            const int id = prog.add_str(s->name);
            fn.emit(OpCode::NOTE_NAME, id);
            return;
        }
        case STMT_STAGE: {
            if (!s->name) {
                vm_compile_error(g_compile_out, "Null STAGE name");
                std::exit(1);
            }
            compile_expr(prog, fn, s->expr);
            const int id = prog.add_str(s->name);
            fn.emit(OpCode::STAGE_NAME, id);
            return;
        }
        case STMT_EXPR:
            compile_expr(prog, fn, s->expr);
            fn.emit(OpCode::POP);
            return;
        case STMT_BRANCH: {
            /* branch(cond) { then } elsewise { else } */
            compile_expr(prog, fn, s->cond);
            const size_t jmp_false = emit_jump(fn, OpCode::JMP_IF_FALSE, 0);

            compile_stmt(prog, fn, s->then_block);

            if (s->else_block) {
                const size_t jmp_end = emit_jump(fn, OpCode::JMP, 0);
                const size_t else_ip = fn.code.size();
                patch_jump(fn, jmp_false, else_ip);
                compile_stmt(prog, fn, s->else_block);
                const size_t end_ip = fn.code.size();
                patch_jump(fn, jmp_end, end_ip);
            } else {
                const size_t end_ip = fn.code.size();
                patch_jump(fn, jmp_false, end_ip);
            }
            return;
        }
        case STMT_REPEAT: {
            /* repeat(cond){ body } */
            const size_t loop_begin = fn.code.size();

            compile_expr(prog, fn, s->cond);
            const size_t jmp_exit = emit_jump(fn, OpCode::JMP_IF_FALSE, 0);

            LoopContext ctx;
            ctx.continue_target = loop_begin;
            g_loops.push_back(ctx);

            compile_stmt(prog, fn, s->body);

            const LoopContext done = g_loops.back();
            g_loops.pop_back();

            fn.emit(OpCode::JMP, (int)loop_begin);

            const size_t loop_end = fn.code.size();
            patch_jump(fn, jmp_exit, loop_end);

            for (size_t bj : done.break_jumps) {
                patch_jump(fn, bj, loop_end);
            }
            return;
        }
        case STMT_BREAK: {
            if (g_loops.empty()) {
                vm_compile_error(g_compile_out, "break used outside repeat");
                std::exit(1);
            }
            const size_t at = emit_jump(fn, OpCode::JMP, 0);
            g_loops.back().break_jumps.push_back(at);
            return;
        }
        case STMT_CONTINUE: {
            if (g_loops.empty()) {
                vm_compile_error(g_compile_out, "continue used outside repeat");
                std::exit(1);
            }
            fn.emit(OpCode::JMP, (int)g_loops.back().continue_target);
            return;
        }
        default:
            vm_compile_error(g_compile_out, "Statement kind not supported yet (current VM milestone)");
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
