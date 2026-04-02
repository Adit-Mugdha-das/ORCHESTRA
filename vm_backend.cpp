#include "interpreter.h"

#include "bytecode.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include "flow_registry.h"

static FILE* g_compile_out = nullptr;

static BytecodeProgram* g_prog = nullptr;
static std::unordered_map<std::string, int> g_func_index;
static std::unordered_map<std::string, bool> g_func_compiling;

static int g_scope_depth = 0; /* lexical block scopes (STMT_BLOCK only) */

static void vm_compile_error(FILE* out, const char* msg) {
    if (!out) out = stdout;
    std::fprintf(out, "VM Compile Error: %s\n", msg ? msg : "<unknown>");
    std::fflush(out);
}

static void compile_expr(BytecodeProgram& prog, BytecodeFunc& fn, Expr* e);
static void compile_stmt(BytecodeProgram& prog, BytecodeFunc& fn, Stmt* s);

struct LoopContext {
    size_t continue_target;        /* SIZE_MAX = patch later (score loop) */
    int scope_depth_at_loop;
    std::vector<size_t> break_jumps;
    std::vector<size_t> continue_jumps; /* for score loops: continue lands at step */
};

static std::vector<LoopContext> g_loops;

static int ensure_compiled_flow(const char* name);

static void precompile_all_flows();

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

/* Jump helpers — also defined again at their original location for any callers below */
static size_t emit_jump_early(BytecodeFunc& fn, OpCode op, int placeholder = 0) {
    const size_t at = fn.code.size();
    fn.emit(op, placeholder);
    return at;
}
static void patch_jump_early(BytecodeFunc& fn, size_t at, size_t target) {
    if (at < fn.code.size()) fn.code[at].a = (int)target;
}

static void compile_binary(BytecodeProgram& prog, BytecodeFunc& fn, Expr* e) {
    /* Short-circuit AND/OR must not evaluate the right side eagerly */
    if (e->op == OP_AND) {
        /* a && b:  if a is false → false (skip b); else → b */
        compile_expr(prog, fn, e->left);
        const size_t jmp_false = emit_jump_early(fn, OpCode::JMP_IF_FALSE, 0);
        compile_expr(prog, fn, e->right);
        const size_t jmp_end = emit_jump_early(fn, OpCode::JMP, 0);
        const size_t short_ip = fn.code.size();
        patch_jump_early(fn, jmp_false, short_ip);
        fn.emit(OpCode::PUSH_BOOL, 0); /* false */
        patch_jump_early(fn, jmp_end, fn.code.size());
        return;
    }
    if (e->op == OP_OR) {
        /* a || b:  if a is true → true (skip b); else → b */
        compile_expr(prog, fn, e->left);
        fn.emit(OpCode::NOT); /* negate so we can use JMP_IF_FALSE */
        const size_t jmp_true = emit_jump_early(fn, OpCode::JMP_IF_FALSE, 0);
        compile_expr(prog, fn, e->right);
        const size_t jmp_end = emit_jump_early(fn, OpCode::JMP, 0);
        const size_t short_ip = fn.code.size();
        patch_jump_early(fn, jmp_true, short_ip);
        fn.emit(OpCode::PUSH_BOOL, 1); /* true */
        patch_jump_early(fn, jmp_end, fn.code.size());
        return;
    }

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
        case EXPR_ADDROF: {
            if (!e->name) { vm_compile_error(g_compile_out, "Null addrof variable name"); std::exit(1); }
            const int id = prog.add_str(e->name);
            fn.emit(OpCode::ADDROF, id);
            return;
        }
        case EXPR_DEREF: {
            compile_expr(prog, fn, e->left);
            fn.emit(OpCode::DEREF_OP);
            return;
        }
        case EXPR_VAR: {
            if (!e->name) {
                vm_compile_error(g_compile_out, "Null variable name");
                std::exit(1);
            }
            const int id = prog.add_str(e->name);
            fn.emit(OpCode::LOAD_NAME, id);
            return;
        }
        case EXPR_ARRAYLIT: {
            int n = 0;
            for (ExprList* p = e->array_elems; p; p = p->next) {
                compile_expr(prog, fn, p->expr);
                n++;
            }
            fn.emit(OpCode::ARRAY_LIT, n);
            return;
        }
        case EXPR_INDEX: {
            compile_expr(prog, fn, e->index_target);
            compile_expr(prog, fn, e->index_expr);
            fn.emit(OpCode::INDEX_GET);
            return;
        }
        case EXPR_CALL: {
            if (!e->callee) {
                vm_compile_error(g_compile_out, "Null callee name");
                std::exit(1);
            }

            /* Built-in collection helpers (match interpreter semantics) */
            if (std::strcmp(e->callee, "array") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 1) {
                    vm_compile_error(g_compile_out, "array(n) expects exactly 1 argument");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                fn.emit(OpCode::ARRAY_NEW);
                return;
            }

            if (std::strcmp(e->callee, "push") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 2) {
                    vm_compile_error(g_compile_out, "push(arr, value) expects exactly 2 arguments");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                compile_expr(prog, fn, e->args->next->expr);
                fn.emit(OpCode::ARRAY_PUSH);
                return;
            }

            if (std::strcmp(e->callee, "pop") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 1) {
                    vm_compile_error(g_compile_out, "pop(arr) expects exactly 1 argument");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                fn.emit(OpCode::ARRAY_POP);
                return;
            }

            if (std::strcmp(e->callee, "resize") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 2) {
                    vm_compile_error(g_compile_out, "resize(arr, newSize) expects exactly 2 arguments");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                compile_expr(prog, fn, e->args->next->expr);
                fn.emit(OpCode::ARRAY_RESIZE);
                return;
            }

            if (std::strcmp(e->callee, "map") == 0) {
                if (e->args) {
                    vm_compile_error(g_compile_out, "map() expects no arguments");
                    std::exit(1);
                }
                fn.emit(OpCode::MAP_NEW);
                return;
            }

            if (std::strcmp(e->callee, "set") == 0) {
                if (e->args) {
                    vm_compile_error(g_compile_out, "set() expects no arguments");
                    std::exit(1);
                }
                fn.emit(OpCode::SET_NEW);
                return;
            }

            if (std::strcmp(e->callee, "add") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 2) {
                    vm_compile_error(g_compile_out, "add(s, key) expects exactly 2 arguments");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                compile_expr(prog, fn, e->args->next->expr);
                fn.emit(OpCode::SET_ADD);
                return;
            }

            if (std::strcmp(e->callee, "has") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 2) {
                    vm_compile_error(g_compile_out, "has(m, key) expects exactly 2 arguments");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                compile_expr(prog, fn, e->args->next->expr);
                fn.emit(OpCode::COLL_HAS);
                return;
            }

            if (std::strcmp(e->callee, "get") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 2) {
                    vm_compile_error(g_compile_out, "get(m, key) expects exactly 2 arguments");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                compile_expr(prog, fn, e->args->next->expr);
                fn.emit(OpCode::MAP_GET);
                return;
            }

            if (std::strcmp(e->callee, "put") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 3) {
                    vm_compile_error(g_compile_out, "put(m, key, value) expects exactly 3 arguments");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                compile_expr(prog, fn, e->args->next->expr);
                compile_expr(prog, fn, e->args->next->next->expr);
                fn.emit(OpCode::MAP_PUT);
                return;
            }

            if (std::strcmp(e->callee, "del") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 2) {
                    vm_compile_error(g_compile_out, "del(m, key) expects exactly 2 arguments");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                compile_expr(prog, fn, e->args->next->expr);
                fn.emit(OpCode::COLL_DEL);
                return;
            }

            if (std::strcmp(e->callee, "keys") == 0) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) argc++;
                if (argc != 1) {
                    vm_compile_error(g_compile_out, "keys(m) expects exactly 1 argument");
                    std::exit(1);
                }
                compile_expr(prog, fn, e->args->expr);
                fn.emit(OpCode::COLL_KEYS);
                return;
            }

            /* If it's a known flow, compile as normal call; otherwise, allow constructor-like TypeName(args). */
            FlowHandle* h = flow_registry_find(e->callee);
            if (h) {
                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) {
                    compile_expr(prog, fn, a->expr);
                    argc++;
                }

                const int fidx = ensure_compiled_flow(e->callee);
                fn.emit(OpCode::CALL, fidx, argc);
                return;
            }

            if (is_ensemble_type(e->callee)) {
                const int type_id = prog.add_str(e->callee);
                fn.emit(OpCode::PUSH_STR, type_id);

                int argc = 0;
                for (ExprList* a = e->args; a; a = a->next) {
                    compile_expr(prog, fn, a->expr);
                    argc++;
                }
                fn.emit(OpCode::CTOR, argc);
                return;
            }

            vm_compile_error(g_compile_out, "Call to undefined flow");
            std::exit(1);
            return;
        }
        case EXPR_FIELD: {
            if (!e->base || !e->field) {
                vm_compile_error(g_compile_out, "Null field access");
                std::exit(1);
            }
            const int base_id = prog.add_str(e->base);
            const int field_id = prog.add_str(e->field);
            fn.emit(OpCode::PUSH_STR, base_id);
            fn.emit(OpCode::PUSH_STR, field_id);
            fn.emit(OpCode::FIELD_GET);
            return;
        }
        case EXPR_DOTCALL: {
            if (!e->dot_base || !e->dot_member) {
                vm_compile_error(g_compile_out, "Null dot call");
                std::exit(1);
            }
            const int base_id = prog.add_str(e->dot_base);
            const int mem_id = prog.add_str(e->dot_member);
            fn.emit(OpCode::PUSH_STR, base_id);
            fn.emit(OpCode::PUSH_STR, mem_id);

            int argc = 0;
            for (ExprList* a = e->dot_args; a; a = a->next) {
                compile_expr(prog, fn, a->expr);
                argc++;
            }
            fn.emit(OpCode::DOTCALL, argc);
            return;
        }
        case EXPR_SUPERCALL: {
            if (!e->super_member) {
                vm_compile_error(g_compile_out, "Null super member");
                std::exit(1);
            }
            const int mem_id = prog.add_str(e->super_member);
            fn.emit(OpCode::PUSH_STR, mem_id);

            int argc = 0;
            for (ExprList* a = e->super_args; a; a = a->next) {
                compile_expr(prog, fn, a->expr);
                argc++;
            }
            fn.emit(OpCode::SUPERCALL, argc);
            return;
        }
        case EXPR_SUPERCTOR: {
            int argc = 0;
            for (ExprList* a = e->super_ctor_args; a; a = a->next) {
                compile_expr(prog, fn, a->expr);
                argc++;
            }
            fn.emit(OpCode::SUPERCTOR, argc);
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
        case STMT_FIXED: {
            /* Treat like NOTE at bytecode level — const enforcement is the AST interpreter's job */
            if (!s->name) { vm_compile_error(g_compile_out, "Null FIXED name"); std::exit(1); }
            compile_expr(prog, fn, s->expr);
            const int id = prog.add_str(s->name);
            fn.emit(OpCode::NOTE_NAME, id);
            return;
        }
        case STMT_PLAY: {
            int argc = 0;
            for (ExprList *a = s->play_args; a; a = a->next) {
                compile_expr(prog, fn, a->expr);
                argc++;
            }
            fn.emit(OpCode::PLAY, argc);
            return;
        }
        case STMT_BLOCK:
            fn.emit(OpCode::SCOPE_PUSH);
            g_scope_depth++;
            compile_stmt_list(prog, fn, s->list);
            fn.emit(OpCode::SCOPE_POP);
            g_scope_depth--;
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
        case STMT_STAGETHRU: {
            if (!s->name) { vm_compile_error(g_compile_out, "Null STAGETHRU pointer name"); std::exit(1); }
            const int id = prog.add_str(s->name);
            fn.emit(OpCode::LOAD_NAME, id);    /* push the pointer value */
            compile_expr(prog, fn, s->expr);   /* push the rhs value    */
            fn.emit(OpCode::STORE_THRU);
            return;
        }
        case STMT_FIELD_STAGE: {
            if (!s->base || !s->field) {
                vm_compile_error(g_compile_out, "Null field stage");
                std::exit(1);
            }
            const int base_id = prog.add_str(s->base);
            const int field_id = prog.add_str(s->field);
            fn.emit(OpCode::PUSH_STR, base_id);
            fn.emit(OpCode::PUSH_STR, field_id);
            compile_expr(prog, fn, s->expr);
            fn.emit(OpCode::FIELD_SET);
            return;
        }
        case STMT_INDEX_STAGE: {
            if (!s->name) {
                vm_compile_error(g_compile_out, "Null index-stage name");
                std::exit(1);
            }

            /* Interpreter requires the target be a variable (array/map/set). */
            const int id = prog.add_str(s->name);
            fn.emit(OpCode::LOAD_NAME, id);
            compile_expr(prog, fn, s->index);
            compile_expr(prog, fn, s->expr);
            fn.emit(OpCode::INDEX_SET);
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
            ctx.scope_depth_at_loop = g_scope_depth;
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
        case STMT_SCORE: {
            /* score (init; cond; step) { body } - classic for-loop */
            /* open scope so 'note i' in init is local to the loop */
            fn.emit(OpCode::SCOPE_PUSH);
            g_scope_depth++;

            if (s->init) compile_stmt(prog, fn, s->init);

            const size_t loop_begin = fn.code.size();

            /* condition — may be absent (infinite until break) */
            size_t jmp_exit = SIZE_MAX;
            if (s->cond) {
                compile_expr(prog, fn, s->cond);
                jmp_exit = emit_jump(fn, OpCode::JMP_IF_FALSE, 0);
            }

            LoopContext ctx;
            ctx.continue_target = SIZE_MAX; /* patched after step is emitted */
            ctx.scope_depth_at_loop = g_scope_depth;
            g_loops.push_back(ctx);

            compile_stmt(prog, fn, s->body);

            LoopContext done = g_loops.back();
            g_loops.pop_back();

            /* step — continue jumps land here */
            const size_t step_ip = fn.code.size();
            for (size_t cj : done.continue_jumps) {
                patch_jump(fn, cj, step_ip);
            }
            if (s->step) compile_stmt(prog, fn, s->step);

            fn.emit(OpCode::JMP, (int)loop_begin);

            const size_t loop_end = fn.code.size();
            if (jmp_exit != SIZE_MAX) patch_jump(fn, jmp_exit, loop_end);
            for (size_t bj : done.break_jumps) {
                patch_jump(fn, bj, loop_end);
            }

            fn.emit(OpCode::SCOPE_POP);
            g_scope_depth--;
            return;
        }
        case STMT_BREAK: {
            if (g_loops.empty()) {
                vm_compile_error(g_compile_out, "break used outside repeat");
                std::exit(1);
            }

            const int target_depth = g_loops.back().scope_depth_at_loop;
            for (int i = 0; i < (g_scope_depth - target_depth); i++) fn.emit(OpCode::SCOPE_POP);
            const size_t at = emit_jump(fn, OpCode::JMP, 0);
            g_loops.back().break_jumps.push_back(at);
            return;
        }
        case STMT_CONTINUE: {
            if (g_loops.empty()) {
                vm_compile_error(g_compile_out, "continue used outside repeat");
                std::exit(1);
            }

            const int target_depth = g_loops.back().scope_depth_at_loop;
            for (int i = 0; i < (g_scope_depth - target_depth); i++) fn.emit(OpCode::SCOPE_POP);

            if (g_loops.back().continue_target == SIZE_MAX) {
                /* score loop: continue must jump to the step; patch later */
                const size_t at = emit_jump(fn, OpCode::JMP, 0);
                g_loops.back().continue_jumps.push_back(at);
            } else {
                fn.emit(OpCode::JMP, (int)g_loops.back().continue_target);
            }
            return;
        }
        case STMT_RETURN: {
            if (s->expr) compile_expr(prog, fn, s->expr);
            else fn.emit(OpCode::PUSH_INT, 0);

            /* Unwind any active block scopes before returning (matches interpreter). */
            for (int i = 0; i < g_scope_depth; i++) fn.emit(OpCode::SCOPE_POP);

            fn.emit(OpCode::RET);
            return;
        }
        default:
            vm_compile_error(g_compile_out, "Statement kind not supported yet (current VM milestone)");
            std::exit(1);
    }
}

static void precompile_cb(const char *name, void * /*user*/) {
    if (!name) return;
    (void)ensure_compiled_flow(name);
}

static void precompile_all_flows() {
    flow_registry_for_each(precompile_cb, nullptr);
}

static int ensure_compiled_flow(const char* name) {
    if (!name || !g_prog) {
        vm_compile_error(g_compile_out, "Internal: compiler not initialized");
        std::exit(1);
    }

    auto it = g_func_index.find(name);
    if (it != g_func_index.end()) return it->second;

    /* Create placeholder index early to support recursion */
    const int idx = (int)g_prog->functions.size();
    g_prog->functions.emplace_back();
    g_func_index[std::string(name)] = idx;
    g_prog->func_name_to_index[std::string(name)] = idx;

    if (g_func_compiling[std::string(name)]) {
        return idx;
    }
    g_func_compiling[std::string(name)] = true;

    FlowHandle* h = flow_registry_find(name);
    if (!h) {
        vm_compile_error(g_compile_out, "Call to undefined flow");
        std::exit(1);
    }

    NameList* params = flow_registry_params(h);
    Stmt* body = flow_registry_body(h);

    BytecodeFunc& fn = g_prog->functions[(size_t)idx];
    fn.code.clear();
    fn.param_name_ids.clear();

    for (NameList* p = params; p; p = p->next) {
        if (!p->name) continue;
        fn.param_name_ids.push_back(g_prog->add_str(p->name));
    }

    const int saved_scope = g_scope_depth;
    g_scope_depth = 0;

    compile_stmt(*g_prog, fn, body);

    /* Default return value when flow ends without explicit return. */
    fn.emit(OpCode::PUSH_INT, 0);
    fn.emit(OpCode::RET);

    g_scope_depth = saved_scope;
    g_func_compiling[std::string(name)] = false;
    return idx;
}

extern "C" int execute_program_vm(Stmt* root, FILE* out) {
    if (!out) out = stdout;
    g_compile_out = out;

    g_prog = nullptr;
    g_func_index.clear();
    g_func_compiling.clear();
    g_loops.clear();
    g_scope_depth = 0;
    if (!root) {
        std::fprintf(out, "VM backend: no entry block\n");
        return 1;
    }

    BytecodeProgram prog;
    g_prog = &prog;

    prog.functions.emplace_back();
    BytecodeFunc& entry = prog.functions.back();

    /* For Milestone 8 dynamic method dispatch (dotcall/super), compile all flows up-front. */
    precompile_all_flows();

    compile_stmt(prog, entry, root);
    entry.emit(OpCode::PUSH_INT, 0);
    entry.emit(OpCode::RET);

    const bool ok = vm_execute(out, prog, entry);
    return ok ? 0 : 1;
}

extern "C" int dump_bytecode_vm(Stmt* root, FILE* out) {
    if (!out) out = stdout;
    g_compile_out = out;

    g_prog = nullptr;
    g_func_index.clear();
    g_func_compiling.clear();
    g_loops.clear();
    g_scope_depth = 0;

    if (!root) {
        std::fprintf(out, "VM backend: no entry block\n");
        return 1;
    }

    BytecodeProgram prog;
    g_prog = &prog;

    prog.functions.emplace_back();
    BytecodeFunc& entry = prog.functions.back();

    precompile_all_flows();

    compile_stmt(prog, entry, root);
    entry.emit(OpCode::PUSH_INT, 0);
    entry.emit(OpCode::RET);

    std::fprintf(out, "== Bytecode (VM) ==\n");
    std::fprintf(out, "\n-- const_num (%zu) --\n", prog.const_num.size());
    for (size_t i = 0; i < prog.const_num.size(); i++) {
        std::fprintf(out, "%zu: %.17g\n", i, prog.const_num[i]);
    }

    std::fprintf(out, "\n-- const_str (%zu) --\n", prog.const_str.size());
    for (size_t i = 0; i < prog.const_str.size(); i++) {
        std::fprintf(out, "%zu: \"%s\"\n", i, prog.const_str[i].c_str());
    }

    std::fprintf(out, "\n-- functions (%zu) --\n", prog.functions.size());
    for (size_t fi = 0; fi < prog.functions.size(); fi++) {
        const BytecodeFunc& fn = prog.functions[fi];
        std::fprintf(out, "\n[func %zu]%s\n", fi, (&fn == &entry) ? " (entry)" : "");

        if (!fn.param_name_ids.empty()) {
            std::fprintf(out, "params:");
            for (size_t pi = 0; pi < fn.param_name_ids.size(); pi++) {
                const int sid = fn.param_name_ids[pi];
                const char* pname = (sid >= 0 && (size_t)sid < prog.const_str.size()) ? prog.const_str[(size_t)sid].c_str() : "?";
                std::fprintf(out, " %s", pname);
            }
            std::fprintf(out, "\n");
        }

        dump_bytecode_func(fn, out);
    }

    std::fflush(out);
    return 0;
}
