#include "interpreter.h"

#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"

typedef struct Value {
    char type[16];
    double num;
    char str[256];
    int boolean;
} Value;

static FILE *g_out = NULL;
static int g_loop_depth = 0;

typedef enum {
    SIG_OK = 0,
    SIG_BREAK,
    SIG_CONTINUE,
    SIG_RETURN
} ExecSignal;

static Value g_return_value;
static int g_has_return = 0;

static ExecSignal exec_stmt(Stmt *s);

typedef struct Flow {
    char *name;
    NameList *params;
    Stmt *body;
} Flow;

#define MAX_FLOWS 128
static Flow g_flows[MAX_FLOWS];
static int g_flow_count = 0;

static Flow* find_flow(const char *name) {
    for (int i = 0; i < g_flow_count; i++) {
        if (strcmp(g_flows[i].name, name) == 0) return &g_flows[i];
    }
    return NULL;
}

static void runtime_error(const char *msg) {
    FILE *out = g_out ? g_out : stdout;
    fprintf(out, "Runtime Error: %s\n", msg);
    exit(1);
}

static int is_numeric_type(const char *t) {
    return (strcmp(t, "int") == 0 || strcmp(t, "float") == 0);
}

static Value value_num(double num, const char *type) {
    Value v;
    strncpy(v.type, type, 15);
    v.type[15] = '\0';
    v.num = num;
    v.str[0] = '\0';
    v.boolean = 0;
    return v;
}

static Value value_bool(int b) {
    Value v;
    strcpy(v.type, "bool");
    v.num = 0;
    v.str[0] = '\0';
    v.boolean = b ? 1 : 0;
    return v;
}

static Value value_string(const char *s) {
    Value v;
    strcpy(v.type, "string");
    v.num = 0;
    v.boolean = 0;
    strncpy(v.str, s ? s : "", sizeof(v.str) - 1);
    v.str[sizeof(v.str) - 1] = '\0';
    return v;
}

Expr* make_lit_num(double num, int is_float) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_LIT;
    strcpy(e->lit_type, is_float ? "float" : "int");
    e->lit_num = num;
    return e;
}

Expr* make_lit_bool(int boolean) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_LIT;
    strcpy(e->lit_type, "bool");
    e->lit_bool = boolean ? 1 : 0;
    return e;
}

Expr* make_lit_string(const char *s) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_LIT;
    strcpy(e->lit_type, "string");
    strncpy(e->lit_str, s ? s : "", sizeof(e->lit_str) - 1);
    e->lit_str[sizeof(e->lit_str) - 1] = '\0';
    return e;
}

Expr* make_var(char *name) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_VAR;
    e->name = name;
    return e;
}

Expr* make_bin(int op, Expr *l, Expr *r) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_BIN;
    e->op = op;
    e->left = l;
    e->right = r;
    return e;
}

Expr* make_unary(int op, Expr *operand) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_UNARY;
    e->op = op;
    e->left = operand;
    e->right = NULL;
    return e;
}

Expr* make_call(char *callee, ExprList *args) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_CALL;
    e->callee = callee;
    e->args = args;
    return e;
}

Expr* make_chain(Expr *first, ChainPart *rest) {
    Expr *e = (Expr*)calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_CHAIN;
    e->chain_first = first;
    e->chain_rest = rest;
    return e;
}

ChainPart* chainpart_append(ChainPart *list, int op, Expr *expr) {
    ChainPart *node = (ChainPart*)calloc(1, sizeof(ChainPart));
    if (!node) exit(1);
    node->op = op;
    node->expr = expr;
    node->next = NULL;
    if (!list) return node;
    ChainPart *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = node;
    return list;
}

ExprList* exprlist_append(ExprList *list, Expr *expr) {
    ExprList *node = (ExprList*)calloc(1, sizeof(ExprList));
    if (!node) exit(1);
    node->expr = expr;
    node->next = NULL;
    if (!list) return node;
    ExprList *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = node;
    return list;
}

NameList* namelist_append(NameList *list, char *name) {
    NameList *node = (NameList*)calloc(1, sizeof(NameList));
    if (!node) exit(1);
    node->name = name;
    node->next = NULL;
    if (!list) return node;
    NameList *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = node;
    return list;
}

StmtList* stmtlist_append(StmtList *list, Stmt *stmt) {
    StmtList *node = (StmtList*)calloc(1, sizeof(StmtList));
    if (!node) exit(1);
    node->stmt = stmt;
    node->next = NULL;

    if (!list) return node;
    StmtList *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = node;
    return list;
}

Stmt* make_block(StmtList *list) {
    Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_BLOCK;
    s->list = list;
    return s;
}

Stmt* make_assign(int kind, char *name, Expr *expr) {
    Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = kind;
    s->name = name;
    s->expr = expr;
    return s;
}

Stmt* make_emit(Expr *expr) {
    Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_EMIT;
    s->expr = expr;
    return s;
}

Stmt* make_expr_stmt(Expr *expr) {
    Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_EXPR;
    s->expr = expr;
    return s;
}

Stmt* make_branch(Expr *cond, Stmt *then_block, Stmt *else_block) {
    Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_BRANCH;
    s->cond = cond;
    s->then_block = then_block;
    s->else_block = else_block;
    return s;
}

Stmt* make_repeat(Expr *cond, Stmt *body) {
    Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_REPEAT;
    s->cond = cond;
    s->body = body;
    return s;
}

Stmt* make_return(Expr *expr) {
    Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_RETURN;
    s->expr = expr;
    return s;
}

void register_flow(char *name, NameList *params, Stmt *body) {
    if (g_flow_count >= MAX_FLOWS) runtime_error("Too many flows");
    if (find_flow(name)) runtime_error("Duplicate flow name");
    g_flows[g_flow_count].name = name;
    g_flows[g_flow_count].params = params;
    g_flows[g_flow_count].body = body;
    g_flow_count++;
}

static Value symbol_to_value(Symbol *sym) {
    if (strcmp(sym->type, "int") == 0) return value_num(sym->num_value, "int");
    if (strcmp(sym->type, "float") == 0) return value_num(sym->num_value, "float");
    if (strcmp(sym->type, "bool") == 0) return value_bool(sym->bool_value);
    if (strcmp(sym->type, "string") == 0) return value_string(sym->str_value);
    runtime_error("Unknown symbol type");
    return value_num(0, "int");
}

static void store_value_note(const char *name, Value v) {
    if (strcmp(v.type, "int") == 0 || strcmp(v.type, "float") == 0)
        declare_or_update_current_scope_value(name, v.type, v.num, NULL, 0);
    else if (strcmp(v.type, "bool") == 0)
        declare_or_update_current_scope_value(name, "bool", 0, NULL, v.boolean);
    else if (strcmp(v.type, "string") == 0)
        declare_or_update_current_scope_value(name, "string", 0, v.str, 0);
    else
        runtime_error("Unsupported value type in note");
}

static void store_value_stage(const char *name, Value v) {
    if (strcmp(v.type, "int") == 0 || strcmp(v.type, "float") == 0)
        insert_or_update_value(name, v.type, v.num, NULL, 0);
    else if (strcmp(v.type, "bool") == 0)
        insert_or_update_value(name, "bool", 0, NULL, v.boolean);
    else if (strcmp(v.type, "string") == 0)
        insert_or_update_value(name, "string", 0, v.str, 0);
    else
        runtime_error("Unsupported value type in stage");
}

static Value eval_bin_numeric(int op, Value a, Value b) {
    if (!is_numeric_type(a.type) || !is_numeric_type(b.type)) runtime_error("Arithmetic requires numeric types");

    int result_is_float = (strcmp(a.type, "float") == 0 || strcmp(b.type, "float") == 0 || op == OP_DIV);
    double x = a.num;
    double y = b.num;
    double r = 0;

    if (op == OP_PLUS) r = x + y;
    else if (op == OP_MINUS) r = x - y;
    else if (op == OP_MUL) r = x * y;
    else if (op == OP_DIV) r = x / y;

    if (result_is_float) return value_num(r, "float");
    return value_num((double)((long long)r), "int");
}

static Value eval_expr(Expr *e);

static Value eval_expr(Expr *e) {
    if (!e) runtime_error("Null expression");

    if (e->kind == EXPR_LIT) {
        if (strcmp(e->lit_type, "int") == 0 || strcmp(e->lit_type, "float") == 0) return value_num(e->lit_num, e->lit_type);
        if (strcmp(e->lit_type, "bool") == 0) return value_bool(e->lit_bool);
        if (strcmp(e->lit_type, "string") == 0) return value_string(e->lit_str);
        runtime_error("Unknown literal type");
    }

    if (e->kind == EXPR_VAR) {
        Symbol *sym = get_symbol_or_error(e->name);
        return symbol_to_value(sym);
    }

    if (e->kind == EXPR_CALL) {
        Flow *flow = find_flow(e->callee);
        if (!flow) runtime_error("Call to undefined flow");

        /* Save/restore return context for nested calls */
        Value prev_ret = g_return_value;
        int prev_has = g_has_return;
        g_has_return = 0;

        push_scope();

        NameList *p = flow->params;
        ExprList *a = e->args;
        while (p && a) {
            Value av = eval_expr(a->expr);
            store_value_note(p->name, av);
            p = p->next;
            a = a->next;
        }
        if (p || a) runtime_error("Argument count mismatch in call");

        ExecSignal sig = exec_stmt(flow->body);
        (void)sig;

        Value outv = g_has_return ? g_return_value : value_num(0, "int");

        pop_scope();

        g_return_value = prev_ret;
        g_has_return = prev_has;

        return outv;
    }

    if (e->kind == EXPR_CHAIN) {
        Value prev = eval_expr(e->chain_first);
        for (ChainPart *p = e->chain_rest; p; p = p->next) {
            Value next = eval_expr(p->expr);
            int ok = 0;

            if (p->op == OP_LT || p->op == OP_LE || p->op == OP_GT || p->op == OP_GE) {
                if (!is_numeric_type(prev.type) || !is_numeric_type(next.type)) {
                    runtime_error("Comparison requires numeric types");
                }
                if (p->op == OP_LT) ok = (prev.num < next.num);
                else if (p->op == OP_LE) ok = (prev.num <= next.num);
                else if (p->op == OP_GT) ok = (prev.num > next.num);
                else ok = (prev.num >= next.num);
            } else if (p->op == OP_EQ || p->op == OP_NE) {
                int eq = 0;
                if (is_numeric_type(prev.type) && is_numeric_type(next.type)) eq = (prev.num == next.num);
                else if (strcmp(prev.type, "bool") == 0 && strcmp(next.type, "bool") == 0) eq = (prev.boolean == next.boolean);
                else if (strcmp(prev.type, "string") == 0 && strcmp(next.type, "string") == 0) eq = (strcmp(prev.str, next.str) == 0);
                else runtime_error("==/!= operands must be comparable");
                ok = (p->op == OP_EQ) ? eq : !eq;
            } else {
                runtime_error("Invalid operator in comparison chain");
            }

            if (!ok) return value_bool(0);
            prev = next;
        }
        return value_bool(1);
    }

    if (e->kind == EXPR_UNARY) {
        Value v = eval_expr(e->left);
        if (e->op == OP_NOT) {
            if (strcmp(v.type, "bool") != 0) runtime_error("! requires bool");
            return value_bool(!v.boolean);
        }
        if (e->op == OP_NEG) {
            if (!is_numeric_type(v.type)) runtime_error("Unary - requires numeric type");
            return value_num(-v.num, v.type);
        }
        runtime_error("Unknown unary operator");
    }

    if (e->kind == EXPR_BIN) {
        if (e->op == OP_AND || e->op == OP_OR) {
            Value left = eval_expr(e->left);
            if (strcmp(left.type, "bool") != 0) runtime_error("Logical operators require bool");

            if (e->op == OP_AND) {
                if (!left.boolean) return value_bool(0);
                Value right = eval_expr(e->right);
                if (strcmp(right.type, "bool") != 0) runtime_error("Logical operators require bool");
                return value_bool(left.boolean && right.boolean);
            }

            if (left.boolean) return value_bool(1);
            Value right = eval_expr(e->right);
            if (strcmp(right.type, "bool") != 0) runtime_error("Logical operators require bool");
            return value_bool(left.boolean || right.boolean);
        }

        Value a = eval_expr(e->left);
        Value b = eval_expr(e->right);

        if (e->op == OP_PLUS) {
            if (strcmp(a.type, "string") == 0 && strcmp(b.type, "string") == 0) {
                Value out = value_string("");
                size_t la = strlen(a.str);
                size_t lb = strlen(b.str);
                if (la + lb >= sizeof(out.str)) runtime_error("String concatenation too long");
                memcpy(out.str, a.str, la);
                memcpy(out.str + la, b.str, lb);
                out.str[la + lb] = '\0';
                return out;
            }
            if (is_numeric_type(a.type) && is_numeric_type(b.type)) {
                return eval_bin_numeric(OP_PLUS, a, b);
            }
            runtime_error("'+' supports numeric addition or string concatenation only");
        }

        if (e->op == OP_MINUS || e->op == OP_MUL || e->op == OP_DIV) {
            return eval_bin_numeric(e->op, a, b);
        }

        if (e->op == OP_LT || e->op == OP_LE || e->op == OP_GT || e->op == OP_GE) {
            if (!is_numeric_type(a.type) || !is_numeric_type(b.type)) runtime_error("Comparison requires numeric types");
            if (e->op == OP_LT) return value_bool(a.num < b.num);
            if (e->op == OP_LE) return value_bool(a.num <= b.num);
            if (e->op == OP_GT) return value_bool(a.num > b.num);
            return value_bool(a.num >= b.num);
        }

        if (e->op == OP_EQ || e->op == OP_NE) {
            int eq = 0;
            if (is_numeric_type(a.type) && is_numeric_type(b.type)) eq = (a.num == b.num);
            else if (strcmp(a.type, "bool") == 0 && strcmp(b.type, "bool") == 0) eq = (a.boolean == b.boolean);
            else if (strcmp(a.type, "string") == 0 && strcmp(b.type, "string") == 0) eq = (strcmp(a.str, b.str) == 0);
            else runtime_error("==/!= operands must be comparable");

            return value_bool(e->op == OP_EQ ? eq : !eq);
        }

        runtime_error("Unknown binary operator");
    }

    runtime_error("Unknown expression kind");
    return value_num(0, "int");
}

static void print_value(Value v) {
    FILE *out = g_out ? g_out : stdout;

    if (strcmp(v.type, "int") == 0) {
        fprintf(out, "%lld\n", (long long)v.num);
        return;
    }
    if (strcmp(v.type, "float") == 0) {
        fprintf(out, "%g\n", v.num);
        return;
    }
    if (strcmp(v.type, "bool") == 0) {
        fprintf(out, "%s\n", v.boolean ? "true" : "false");
        return;
    }
    if (strcmp(v.type, "string") == 0) {
        fprintf(out, "%s\n", v.str);
        return;
    }

    runtime_error("Unknown value type for emit");
}

static ExecSignal exec_stmt(Stmt *s);

static ExecSignal exec_stmt_list(StmtList *list) {
    for (StmtList *cur = list; cur; cur = cur->next) {
        ExecSignal sig = exec_stmt(cur->stmt);
        if (sig != SIG_OK) return sig;
    }
    return SIG_OK;
}

static ExecSignal exec_stmt(Stmt *s) {
    if (!s) return SIG_OK;

    switch (s->kind) {
        case STMT_NOTE: {
            Value v = eval_expr(s->expr);
            store_value_note(s->name, v);
            return SIG_OK;
        }
        case STMT_STAGE: {
            Value v = eval_expr(s->expr);
            store_value_stage(s->name, v);
            return SIG_OK;
        }
        case STMT_EMIT: {
            Value v = eval_expr(s->expr);
            print_value(v);
            return SIG_OK;
        }
        case STMT_EXPR: {
            (void)eval_expr(s->expr);
            return SIG_OK;
        }
        case STMT_BLOCK: {
            push_scope();
            ExecSignal sig = exec_stmt_list(s->list);
            pop_scope();
            return sig;
        }
        case STMT_BRANCH: {
            Value c = eval_expr(s->cond);
            if (strcmp(c.type, "bool") != 0) runtime_error("branch condition must be bool");
            if (c.boolean) return exec_stmt(s->then_block);
            if (s->else_block) return exec_stmt(s->else_block);
            return SIG_OK;
        }
        case STMT_REPEAT: {
            g_loop_depth++;
            for (;;) {
                Value c = eval_expr(s->cond);
                if (strcmp(c.type, "bool") != 0) runtime_error("repeat condition must be bool");
                if (!c.boolean) break;
                ExecSignal sig = exec_stmt(s->body);
                if (sig == SIG_BREAK) { sig = SIG_OK; break; }
                if (sig == SIG_CONTINUE) { continue; }
                if (sig == SIG_RETURN) { g_loop_depth--; return SIG_RETURN; }
            }
            g_loop_depth--;
            return SIG_OK;
        }
        case STMT_BREAK: {
            if (g_loop_depth <= 0) runtime_error("break used outside repeat");
            return SIG_BREAK;
        }
        case STMT_CONTINUE: {
            if (g_loop_depth <= 0) runtime_error("continue used outside repeat");
            return SIG_CONTINUE;
        }
        case STMT_RETURN: {
            if (s->expr) g_return_value = eval_expr(s->expr);
            else g_return_value = value_num(0, "int");
            g_has_return = 1;
            return SIG_RETURN;
        }
        default:
            runtime_error("Unknown statement kind");
    }

    return SIG_OK;
}

void execute_program(Stmt *root, FILE *out) {
    g_out = out;
    ExecSignal sig = exec_stmt(root);
    if (sig == SIG_BREAK) runtime_error("break used outside repeat");
    if (sig == SIG_CONTINUE) runtime_error("continue used outside repeat");
    /* SIG_RETURN at top-level is allowed: it just stops execution. */
}

static void free_stmt_list(StmtList *list);
static void free_expr(Expr *e);

static void free_expr(Expr *e) {
    if (!e) return;

    if (e->kind == EXPR_VAR) {
        free(e->name);
        e->name = NULL;
    }

    if (e->kind == EXPR_CALL) {
        free(e->callee);
        e->callee = NULL;
        ExprList *cur = e->args;
        while (cur) {
            ExprList *next = cur->next;
            free_expr(cur->expr);
            free(cur);
            cur = next;
        }
        e->args = NULL;
    }

    if (e->kind == EXPR_CHAIN) {
        free_expr(e->chain_first);
        e->chain_first = NULL;
        ChainPart *p = e->chain_rest;
        while (p) {
            ChainPart *next = p->next;
            free_expr(p->expr);
            free(p);
            p = next;
        }
        e->chain_rest = NULL;
    }

    free_expr(e->left);
    free_expr(e->right);
    free(e);
}

static void free_stmt_list(StmtList *list) {
    while (list) {
        StmtList *next = list->next;
        free_stmt(list->stmt);
        free(list);
        list = next;
    }
}

void free_stmt(Stmt *s) {
    if (!s) return;

    if (s->name) {
        free(s->name);
        s->name = NULL;
    }

    free_expr(s->expr);
    free_expr(s->cond);

    if (s->kind == STMT_BLOCK) {
        free_stmt_list(s->list);
    }

    free_stmt(s->then_block);
    free_stmt(s->else_block);
    free_stmt(s->body);

    free(s);
}

static void free_namelist(NameList *list) {
    while (list) {
        NameList *next = list->next;
        free(list->name);
        free(list);
        list = next;
    }
}

void free_all_flows(void) {
    for (int i = 0; i < g_flow_count; i++) {
        free(g_flows[i].name);
        free_namelist(g_flows[i].params);
        free_stmt(g_flows[i].body);
        g_flows[i].name = NULL;
        g_flows[i].params = NULL;
        g_flows[i].body = NULL;
    }
    g_flow_count = 0;
}
