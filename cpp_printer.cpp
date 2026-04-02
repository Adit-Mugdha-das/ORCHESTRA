#include "cpp_printer.h"

#include <cstring>

namespace {

static OrchCppEmitStyle g_style = ORCH_CPP_STYLE_CPP;

static void indent(FILE* out, int n) {
    for (int i = 0; i < n; i++) fputc(' ', out);
}

static void print_string_escaped(FILE* out, const char* s) {
    fputc('"', out);
    for (const unsigned char* p = (const unsigned char*)s; *p; ++p) {
        switch (*p) {
            case '\\': fputs("\\\\", out); break;
            case '"': fputs("\\\"", out); break;
            case '\n': fputs("\\n", out); break;
            case '\r': fputs("\\r", out); break;
            case '\t': fputs("\\t", out); break;
            default:
                if (*p < 32) {
                    fputs("\\x", out);
                    static const char* hex = "0123456789ABCDEF";
                    fputc(hex[(*p >> 4) & 0xF], out);
                    fputc(hex[*p & 0xF], out);
                } else {
                    fputc(*p, out);
                }
                break;
        }
    }
    fputc('"', out);
}

static int precedence_for_binop(int op) {
    switch (op) {
        case OP_OR: return 1;
        case OP_AND: return 2;
        case OP_EQ:
        case OP_NE:
        case OP_LT:
        case OP_LE:
        case OP_GT:
        case OP_GE:
            return 3;
        case OP_PLUS:
        case OP_MINUS:
            return 4;
        case OP_MUL:
        case OP_DIV:
            return 5;
        default:
            return 0;
    }
}

static const char* op_to_cpp(int op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_LT: return "<";
        case OP_LE: return "<=";
        case OP_GT: return ">";
        case OP_GE: return ">=";
        case OP_EQ: return "==";
        case OP_NE: return "!=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        default: return "/*op?*/";
    }
}

static void print_expr_cpp(FILE* out, Expr* e, int parentPrec);

static void print_args_cpp(FILE* out, ExprList* args) {
    bool first = true;
    for (ExprList* p = args; p; p = p->next) {
        if (!first) fputs(", ", out);
        first = false;
        print_expr_cpp(out, p->expr, 0);
    }
}

static void print_chain_cpp(FILE* out, Expr* e, int parentPrec) {
    // Translate a < b < c into (a < b) && (b < c)
    // Safe/readable: always parenthesize the whole chain.
    const bool needParens = parentPrec > 0;
    if (needParens) fputc('(', out);

    Expr* left = e->chain_first;
    ChainPart* part = e->chain_rest;

    bool firstCmp = true;
    while (part) {
        if (!firstCmp) fputs(" && ", out);
        firstCmp = false;

        fputc('(', out);
        print_expr_cpp(out, left, 3);
        fputc(' ', out);
        fputs(op_to_cpp(part->op), out);
        fputc(' ', out);
        print_expr_cpp(out, part->expr, 3);
        fputc(')', out);

        left = part->expr;
        part = part->next;
    }

    if (needParens) fputc(')', out);
}

static void print_expr_cpp(FILE* out, Expr* e, int parentPrec) {
    if (!e) {
        fputs("/*null*/", out);
        return;
    }

    switch (e->kind) {
        case EXPR_LIT: {
            if (std::strcmp(e->lit_type, "string") == 0) {
                print_string_escaped(out, e->lit_str);
            } else if (std::strcmp(e->lit_type, "bool") == 0) {
                fputs(e->lit_bool ? "true" : "false", out);
            } else if (std::strcmp(e->lit_type, "int") == 0) {
                long long v = (long long)e->lit_num;
                fprintf(out, "%lld", v);
            } else if (std::strcmp(e->lit_type, "float") == 0) {
                fprintf(out, "%.15g", e->lit_num);
            } else {
                // Fallback
                fprintf(out, "%.15g", e->lit_num);
            }
            return;
        }

        case EXPR_VAR: {
            fputs(e->name ? e->name : "/*var?*/", out);
            return;
        }

        case EXPR_FIELD: {
            const char* base = e->base ? e->base : "/*base?*/";
            const char* field = e->field ? e->field : "/*field?*/";
            fprintf(out, "%s.%s", base, field);
            return;
        }

        case EXPR_DOTCALL: {
            const char* base = e->dot_base ? e->dot_base : "/*base?*/";
            const char* member = e->dot_member ? e->dot_member : "/*member?*/";
            fprintf(out, "%s.%s(", base, member);
            print_args_cpp(out, e->dot_args);
            fputc(')', out);
            return;
        }

        case EXPR_SUPERCALL: {
            const char* member = e->super_member ? e->super_member : "/*member?*/";
            fputs("/*super*/ super.", out);
            fputs(member, out);
            fputc('(', out);
            print_args_cpp(out, e->super_args);
            fputc(')', out);
            return;
        }

        case EXPR_SUPERCTOR: {
            fputs("/*super*/ super(", out);
            print_args_cpp(out, e->super_ctor_args);
            fputc(')', out);
            return;
        }

        case EXPR_ARRAYLIT: {
            fputs("Array{", out);
            print_args_cpp(out, e->array_elems);
            fputc('}', out);
            return;
        }

        case EXPR_INDEX: {
            print_expr_cpp(out, e->index_target, 7);
            fputc('[', out);
            print_expr_cpp(out, e->index_expr, 0);
            fputc(']', out);
            return;
        }

        case EXPR_ADDROF: {
            fputc('&', out);
            fputs(e->name ? e->name : "/*name?*/", out);
            return;
        }

        case EXPR_DEREF: {
            fputs("deref(", out);
            print_expr_cpp(out, e->left, 0);
            fputc(')', out);
            return;
        }

        case EXPR_UNARY: {
            const int prec = 6;
            const bool needParens = prec < parentPrec;
            if (needParens) fputc('(', out);

            if (e->op == OP_NOT) fputc('!', out);
            else if (e->op == OP_NEG) fputc('-', out);
            else fputs("/*unary?*/", out);

            print_expr_cpp(out, e->left, prec);

            if (needParens) fputc(')', out);
            return;
        }

        case EXPR_BIN: {
            const int prec = precedence_for_binop(e->op);
            const bool needParens = prec < parentPrec;
            if (needParens) fputc('(', out);

            print_expr_cpp(out, e->left, prec);
            fputc(' ', out);
            fputs(op_to_cpp(e->op), out);
            fputc(' ', out);
            print_expr_cpp(out, e->right, prec);

            if (needParens) fputc(')', out);
            return;
        }

        case EXPR_CALL: {
            const char* callee = e->callee ? e->callee : "/*callee?*/";
            fputs(callee, out);
            fputc('(', out);
            print_args_cpp(out, e->args);
            fputc(')', out);
            return;
        }

        case EXPR_CHAIN: {
            print_chain_cpp(out, e, parentPrec);
            return;
        }

        default:
            fprintf(out, "/*unsupported EXPR kind=%d*/", e->kind);
            return;
    }
}

static void print_emit_name(FILE* out) {
    if (g_style == ORCH_CPP_STYLE_PSEUDO) fputs("print", out);
    else fputs("emit", out);
}

static void print_note_keyword(FILE* out) {
    if (g_style == ORCH_CPP_STYLE_PSEUDO) fputs("var", out);
    else fputs("auto", out);
}

static void print_stmt_cpp(FILE* out, Stmt* s, int indentLevel);

static void print_stmt_list_cpp(FILE* out, StmtList* list, int indentLevel) {
    for (StmtList* p = list; p; p = p->next) {
        print_stmt_cpp(out, p->stmt, indentLevel);
    }
}

static void print_block_like(FILE* out, Stmt* blockStmt, int indentLevel) {
    // The parser builds blocks as STMT_BLOCK.
    if (!blockStmt) {
        fputs("{ }", out);
        return;
    }
    print_stmt_cpp(out, blockStmt, indentLevel);
}

static void print_stmt_cpp(FILE* out, Stmt* s, int indentLevel) {
    if (!s) return;

    switch (s->kind) {
        case STMT_NOTE: {
            indent(out, indentLevel);
            print_note_keyword(out);
            fputc(' ', out);
            fputs(s->name ? s->name : "/*name?*/", out);
            fputs(" = ", out);
            print_expr_cpp(out, s->expr, 0);
            fputs(";\n", out);
            return;
        }

        case STMT_FIXED: {
            indent(out, indentLevel);
            fputs("const auto ", out);
            fputs(s->name ? s->name : "/*name?*/", out);
            fputs(" = ", out);
            print_expr_cpp(out, s->expr, 0);
            fputs(";\n", out);
            return;
        }

        case STMT_PLAY: {
            indent(out, indentLevel);
            if (!s->play_args) { fputs("/* play(); */\n", out); return; }
            if (!s->play_args->next) {
                /* Single arg — printf without format string */
                fputs("printf(\"%s\", ", out);  /* safe fallback — show as string */
                /* Actually emit the raw value with to_string equivalent */
                fputs("std::to_string(", out);
                print_expr_cpp(out, s->play_args->expr, 0);
                fputs("));\n", out);
            } else {
                /* Format string + args */
                fputs("printf(", out);
                print_expr_cpp(out, s->play_args->expr, 0);
                for (ExprList *a = s->play_args->next; a; a = a->next) {
                    fputs(", ", out);
                    print_expr_cpp(out, a->expr, 0);
                }
                fputs(");\n", out);
            }
            return;
        }

        case STMT_STAGE: {
            indent(out, indentLevel);
            fputs(s->name ? s->name : "/*name?*/", out);
            fputs(" = ", out);
            print_expr_cpp(out, s->expr, 0);
            fputs(";\n", out);
            return;
        }

        case STMT_FIELD_STAGE: {
            indent(out, indentLevel);
            const char* base = s->base ? s->base : "/*base?*/";
            const char* field = s->field ? s->field : "/*field?*/";
            fprintf(out, "%s.%s = ", base, field);
            print_expr_cpp(out, s->expr, 0);
            fputs(";\n", out);
            return;
        }

        case STMT_INDEX_STAGE: {
            indent(out, indentLevel);
            fputs(s->name ? s->name : "/*name?*/", out);
            fputc('[', out);
            print_expr_cpp(out, s->index, 0);
            fputs("] = ", out);
            print_expr_cpp(out, s->expr, 0);
            fputs(";\n", out);
            return;
        }

        case STMT_STAGETHRU: {
            indent(out, indentLevel);
            fputs("stagethru ", out);
            fputs(s->name ? s->name : "/*name?*/", out);
            fputs(" = ", out);
            print_expr_cpp(out, s->expr, 0);
            fputs(";\n", out);
            return;
        }

        case STMT_EMIT: {
            indent(out, indentLevel);
            print_emit_name(out);
            fputc('(', out);
            print_expr_cpp(out, s->expr, 0);
            fputs(");\n", out);
            return;
        }

        case STMT_EXPR: {
            indent(out, indentLevel);
            print_expr_cpp(out, s->expr, 0);
            fputs(";\n", out);
            return;
        }

        case STMT_BLOCK: {
            indent(out, indentLevel);
            fputs("{\n", out);
            print_stmt_list_cpp(out, s->list, indentLevel + 2);
            indent(out, indentLevel);
            fputs("}\n", out);
            return;
        }

        case STMT_BRANCH: {
            indent(out, indentLevel);
            fputs("if (", out);
            print_expr_cpp(out, s->cond, 0);
            fputs(") ", out);
            print_block_like(out, s->then_block, indentLevel);
            if (s->else_block) {
                // then_block prints trailing newline; align else nicely
                indent(out, indentLevel);
                fputs("else ", out);
                print_block_like(out, s->else_block, indentLevel);
            }
            return;
        }

        case STMT_REPEAT: {
            indent(out, indentLevel);
            fputs("while (", out);
            print_expr_cpp(out, s->cond, 0);
            fputs(") ", out);
            print_block_like(out, s->body, indentLevel);
            return;
        }

        case STMT_SCORE: {
            /* score (init; cond; step) { body }  →  for (init; cond; step) { body } */
            indent(out, indentLevel);
            fputs("for (", out);

            /* init part — print inline without trailing newline */
            if (s->init) {
                if (s->init->kind == STMT_NOTE) {
                    print_note_keyword(out);
                    fputc(' ', out);
                    fputs(s->init->name ? s->init->name : "/*?*/", out);
                    fputs(" = ", out);
                    print_expr_cpp(out, s->init->expr, 0);
                } else if (s->init->kind == STMT_STAGE) {
                    fputs(s->init->name ? s->init->name : "/*?*/", out);
                    fputs(" = ", out);
                    print_expr_cpp(out, s->init->expr, 0);
                }
            }
            fputs("; ", out);

            /* cond part */
            if (s->cond) print_expr_cpp(out, s->cond, 0);
            fputs("; ", out);

            /* step part */
            if (s->step) {
                if (s->step->kind == STMT_STAGE) {
                    fputs(s->step->name ? s->step->name : "/*?*/", out);
                    fputs(" = ", out);
                    print_expr_cpp(out, s->step->expr, 0);
                } else if (s->step->kind == STMT_INDEX_STAGE) {
                    fputs(s->step->name ? s->step->name : "/*?*/", out);
                    fputs("[", out);
                    print_expr_cpp(out, s->step->index, 0);
                    fputs("] = ", out);
                    print_expr_cpp(out, s->step->expr, 0);
                }
            }
            fputs(") ", out);
            print_block_like(out, s->body, indentLevel);
            return;
        }

        case STMT_BREAK: {
            indent(out, indentLevel);
            fputs("break;\n", out);
            return;
        }

        case STMT_CONTINUE: {
            indent(out, indentLevel);
            fputs("continue;\n", out);
            return;
        }

        case STMT_RETURN: {
            indent(out, indentLevel);
            if (s->expr) {
                fputs("return ", out);
                print_expr_cpp(out, s->expr, 0);
                fputs(";\n", out);
            } else {
                fputs("return;\n", out);
            }
            return;
        }

        default:
            indent(out, indentLevel);
            fprintf(out, "/*unsupported STMT kind=%d*/\n", s->kind);
            return;
    }
}

} // namespace

extern "C" void dump_cpp_program_with_style(Stmt* mainBlock, FILE* out, OrchCppEmitStyle style) {
    if (!out) return;

    g_style = style;

    fputs("// ORCHESTRA \xE2\x86\x92 C++ (pretty-print)\n", out);
    fputs("// NOTE: for learning only, not guaranteed to compile\n\n", out);

    fputs("// Legend (ORCHESTRA -> shown here)\n", out);
    if (g_style == ORCH_CPP_STYLE_PSEUDO) {
        fputs("//   note x = expr;              -> var x = expr;\n", out);
        fputs("//   stage x = expr;             -> x = expr;\n", out);
        fputs("//   emit expr;                  -> print(expr);\n", out);
    } else {
        fputs("//   note x = expr;              -> auto x = expr;\n", out);
        fputs("//   stage x = expr;             -> x = expr;\n", out);
        fputs("//   emit expr;                  -> emit(expr);\n", out);
    }
    fputs("//   branch(cond) {...} elsewise -> if(cond) {...} else {...}\n", out);
    fputs("//   repeat(cond) {...}          -> while(cond) {...}\n\n", out);

    fputs("int main() ", out);
    if (!mainBlock) {
        fputs("{\n  return 0;\n}\n", out);
        return;
    }

    // mainBlock is a STMT_BLOCK produced by the parser.
    print_stmt_cpp(out, mainBlock, 0);
}

extern "C" void dump_cpp_program(Stmt* mainBlock, FILE* out) {
    dump_cpp_program_with_style(mainBlock, out, ORCH_CPP_STYLE_CPP);
}
