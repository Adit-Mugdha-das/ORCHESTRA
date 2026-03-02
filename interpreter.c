#include "interpreter.h"

#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"
#include "flow_registry.h"

/* -------------------------
   AST/identifier arena
   ------------------------- */

static void **g_arena_ptrs = NULL;
static size_t g_arena_count = 0;
static size_t g_arena_cap = 0;

static void arena_track(void *p) {
    if (!p) return;
    if (g_arena_count == g_arena_cap) {
        size_t new_cap = g_arena_cap ? (g_arena_cap * 2) : 1024;
        void **np = (void**)realloc(g_arena_ptrs, new_cap * sizeof(void*));
        if (!np) exit(1);
        g_arena_ptrs = np;
        g_arena_cap = new_cap;
    }
    g_arena_ptrs[g_arena_count++] = p;
}

static void* arena_calloc(size_t count, size_t size) {
    void *p = calloc(count, size);
    if (!p) exit(1);
    arena_track(p);
    return p;
}

char* arena_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char *p = (char*)malloc(n + 1);
    if (!p) exit(1);
    memcpy(p, s, n + 1);
    arena_track(p);
    return p;
}

void arena_free_all(void) {
    for (size_t i = 0; i < g_arena_count; i++) {
        free(g_arena_ptrs[i]);
    }
    free(g_arena_ptrs);
    g_arena_ptrs = NULL;
    g_arena_count = 0;
    g_arena_cap = 0;
}

typedef struct Value {
    char type[16];
    double num;
    char str[256];
    int boolean;
    void *ptr;
    char struct_type[64];
} Value;

static FILE *g_out = NULL;
static int g_loop_depth = 0;
static int g_call_depth = 0;

/* Hard guard against infinite recursion / runaway mutual recursion. */
/*
    Keep this fairly low: the interpreter uses real C call stack frames per flow
    call, so a huge limit can still stack-overflow before the guard triggers.
*/
static const int g_max_call_depth = 200;

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

/* Flow storage is implemented in C++ (flow_registry.cpp). */

static FlowHandle* find_flow_handle(const char *name) {
    return flow_registry_find(name);
}

static void runtime_error(const char *msg) {
    FILE *out = g_out ? g_out : stdout;
    fprintf(out, "Runtime Error: %s\n", msg);
    fflush(out);

    /* Also print to stderr so users see errors when running from the console. */
    if (out != stderr) {
        fprintf(stderr, "Runtime Error: %s\n", msg);
        fflush(stderr);
    }
    exit(1);
}

static void recursion_error(const char *callee) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Recursion limit exceeded (max %d) while calling '%s'", g_max_call_depth, callee ? callee : "<unknown>");
    runtime_error(buf);
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
    v.ptr = NULL;
    v.struct_type[0] = '\0';
    return v;
}

static Value value_bool(int b) {
    Value v;
    strcpy(v.type, "bool");
    v.num = 0;
    v.str[0] = '\0';
    v.boolean = b ? 1 : 0;
    v.ptr = NULL;
    v.struct_type[0] = '\0';
    return v;
}

static Value value_string(const char *s) {
    Value v;
    strcpy(v.type, "string");
    v.num = 0;
    v.boolean = 0;
    strncpy(v.str, s ? s : "", sizeof(v.str) - 1);
    v.str[sizeof(v.str) - 1] = '\0';
    v.ptr = NULL;
    v.struct_type[0] = '\0';
    return v;
}

static Value value_struct(const char *type_name, void *ptr) {
    Value v;
    strcpy(v.type, "struct");
    v.num = 0;
    v.str[0] = '\0';
    v.boolean = 0;
    v.ptr = ptr;
    strncpy(v.struct_type, type_name ? type_name : "", sizeof(v.struct_type) - 1);
    v.struct_type[sizeof(v.struct_type) - 1] = '\0';
    return v;
}

/* -------------------------
   Ensemble (struct) registry + instances
   ------------------------- */

typedef struct EnsembleDef {
    char *name;
    FieldList *fields;
    struct EnsembleDef *next;
} EnsembleDef;

typedef struct StructFieldValue {
    char *name;
    Value value;
    struct StructFieldValue *next;
} StructFieldValue;

typedef struct StructInstance {
    char *type_name;
    StructFieldValue *fields;
} StructInstance;

static EnsembleDef *g_ensembles = NULL;

static EnsembleDef* find_ensemble_def(const char *name) {
    for (EnsembleDef *e = g_ensembles; e; e = e->next) {
        if (strcmp(e->name, name) == 0) return e;
    }
    return NULL;
}

FieldList* fieldlist_append(FieldList *list, char *type, char *name) {
    FieldList *node = (FieldList*)arena_calloc(1, sizeof(FieldList));
    if (!node) exit(1);
    node->type = type;
    node->name = name;
    node->next = NULL;
    if (!list) return node;
    FieldList *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = node;
    return list;
}

void register_ensemble(char *name, FieldList *fields) {
    if (!name) runtime_error("Null ensemble name");
    if (find_ensemble_def(name)) runtime_error("Duplicate ensemble name");

    EnsembleDef *node = (EnsembleDef*)arena_calloc(1, sizeof(EnsembleDef));
    if (!node) exit(1);
    node->name = name;
    node->fields = fields;
    node->next = g_ensembles;
    g_ensembles = node;
}

int is_ensemble_type(const char *name) {
    return find_ensemble_def(name) ? 1 : 0;
}

static StructFieldValue* find_struct_field(StructInstance *inst, const char *field) {
    for (StructFieldValue *f = inst ? inst->fields : NULL; f; f = f->next) {
        if (strcmp(f->name, field) == 0) return f;
    }
    return NULL;
}

static StructInstance* make_struct_instance(const char *type_name) {
    EnsembleDef *def = find_ensemble_def(type_name);
    if (!def) runtime_error("Unknown ensemble type");

    StructInstance *inst = (StructInstance*)arena_calloc(1, sizeof(StructInstance));
    if (!inst) exit(1);
    inst->type_name = arena_strdup(type_name);
    inst->fields = NULL;

    StructFieldValue *tail = NULL;

    for (FieldList *f = def->fields; f; f = f->next) {
        StructFieldValue *fv = (StructFieldValue*)arena_calloc(1, sizeof(StructFieldValue));
        if (!fv) exit(1);
        fv->name = f->name;
        if (strcmp(f->type, "int") == 0) fv->value = value_num(0, "int");
        else if (strcmp(f->type, "float") == 0) fv->value = value_num(0, "float");
        else if (strcmp(f->type, "bool") == 0) fv->value = value_bool(0);
        else if (strcmp(f->type, "string") == 0) fv->value = value_string("");
        else runtime_error("Unsupported ensemble field type");

        /* Preserve definition order for emit pretty-printing. */
        fv->next = NULL;
        if (!inst->fields) {
            inst->fields = fv;
            tail = fv;
        } else {
            tail->next = fv;
            tail = fv;
        }
    }

    return inst;
}

static int fieldlist_length(FieldList *f) {
    int n = 0;
    for (; f; f = f->next) n++;
    return n;
}

static int exprlist_length(ExprList *e) {
    int n = 0;
    for (; e; e = e->next) n++;
    return n;
}

Expr* make_lit_num(double num, int is_float) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_LIT;
    strcpy(e->lit_type, is_float ? "float" : "int");
    e->lit_num = num;
    return e;
}

Expr* make_lit_bool(int boolean) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_LIT;
    strcpy(e->lit_type, "bool");
    e->lit_bool = boolean ? 1 : 0;
    return e;
}

Expr* make_lit_string(const char *s) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_LIT;
    strcpy(e->lit_type, "string");
    strncpy(e->lit_str, s ? s : "", sizeof(e->lit_str) - 1);
    e->lit_str[sizeof(e->lit_str) - 1] = '\0';
    return e;
}

Expr* make_var(char *name) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_VAR;
    e->name = name;
    return e;
}

Expr* make_field(char *base, char *field) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_FIELD;
    e->base = base;
    e->field = field;
    return e;
}

Expr* make_dotcall(char *base, char *member, ExprList *args) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_DOTCALL;
    e->dot_base = base;
    e->dot_member = member;
    e->dot_args = args;
    return e;
}

Expr* make_bin(int op, Expr *l, Expr *r) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_BIN;
    e->op = op;
    e->left = l;
    e->right = r;
    return e;
}

Expr* make_unary(int op, Expr *operand) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_UNARY;
    e->op = op;
    e->left = operand;
    e->right = NULL;
    return e;
}

Expr* make_call(char *callee, ExprList *args) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_CALL;
    e->callee = callee;
    e->args = args;
    return e;
}

Expr* make_chain(Expr *first, ChainPart *rest) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_CHAIN;
    e->chain_first = first;
    e->chain_rest = rest;
    return e;
}

ChainPart* chainpart_append(ChainPart *list, int op, Expr *expr) {
    ChainPart *node = (ChainPart*)arena_calloc(1, sizeof(ChainPart));
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
    ExprList *node = (ExprList*)arena_calloc(1, sizeof(ExprList));
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
    NameList *node = (NameList*)arena_calloc(1, sizeof(NameList));
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
    StmtList *node = (StmtList*)arena_calloc(1, sizeof(StmtList));
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
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_BLOCK;
    s->list = list;
    return s;
}

Stmt* make_assign(int kind, char *name, Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = kind;
    s->name = name;
    s->expr = expr;
    return s;
}

Stmt* make_emit(Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_EMIT;
    s->expr = expr;
    return s;
}

Stmt* make_expr_stmt(Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_EXPR;
    s->expr = expr;
    return s;
}

Stmt* make_field_stage(char *base, char *field, Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_FIELD_STAGE;
    s->base = base;
    s->field = field;
    s->expr = expr;
    return s;
}

Stmt* make_branch(Expr *cond, Stmt *then_block, Stmt *else_block) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_BRANCH;
    s->cond = cond;
    s->then_block = then_block;
    s->else_block = else_block;
    return s;
}

Stmt* make_repeat(Expr *cond, Stmt *body) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_REPEAT;
    s->cond = cond;
    s->body = body;
    return s;
}

Stmt* make_return(Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_RETURN;
    s->expr = expr;
    return s;
}

void register_flow(char *name, NameList *params, Stmt *body) {
    flow_registry_register(name, params, body);
}

static Value symbol_to_value(Symbol *sym) {
    if (strcmp(sym->type, "int") == 0) return value_num(sym->num_value, "int");
    if (strcmp(sym->type, "float") == 0) return value_num(sym->num_value, "float");
    if (strcmp(sym->type, "bool") == 0) return value_bool(sym->bool_value);
    if (strcmp(sym->type, "string") == 0) return value_string(sym->str_value);
    if (strcmp(sym->type, "struct") == 0) return value_struct(sym->struct_type, sym->ptr_value);
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
    else if (strcmp(v.type, "struct") == 0)
        declare_or_update_current_scope_struct(name, v.struct_type, v.ptr);
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
    else if (strcmp(v.type, "struct") == 0)
        insert_or_update_struct(name, v.struct_type, v.ptr);
    else
        runtime_error("Unsupported value type in stage");
}

static Value coerce_to_field_type(Value v, const char *field_type) {
    if (!field_type) runtime_error("Unknown field type");

    if (strcmp(field_type, "int") == 0) {
        if (!is_numeric_type(v.type)) runtime_error("Field assignment requires numeric type");
        return value_num((double)((long long)v.num), "int");
    }
    if (strcmp(field_type, "float") == 0) {
        if (!is_numeric_type(v.type)) runtime_error("Field assignment requires numeric type");
        return value_num(v.num, "float");
    }
    if (strcmp(field_type, "bool") == 0) {
        if (strcmp(v.type, "bool") != 0) runtime_error("Field assignment requires bool");
        return value_bool(v.boolean);
    }
    if (strcmp(field_type, "string") == 0) {
        if (strcmp(v.type, "string") != 0) runtime_error("Field assignment requires string");
        return value_string(v.str);
    }
    runtime_error("Unsupported field type");
    return value_num(0, "int");
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

static Value eval_flow_call_handle(FlowHandle *flow, const char *callee, ExprList *args, int has_implicit, Value implicit) {
    if (!flow) runtime_error("Call to undefined flow");

    if (g_call_depth >= g_max_call_depth) {
        free(flow);
        recursion_error(callee);
    }
    g_call_depth++;

    /* Save/restore return context for nested calls */
    Value prev_ret = g_return_value;
    int prev_has = g_has_return;
    g_has_return = 0;

    push_scope();

    NameList *p = flow_registry_params(flow);
    ExprList *a = args;

    if (has_implicit) {
        /* Always provide `this` inside the method body. */
        store_value_note("this", implicit);

        /* Back-compat: if method declares a first parameter, also bind it. */
        if (p) {
            store_value_note(p->name, implicit);
            p = p->next;
        }
    }

    while (p && a) {
        Value av = eval_expr(a->expr);
        store_value_note(p->name, av);
        p = p->next;
        a = a->next;
    }
    if (p || a) runtime_error("Argument count mismatch in call");

    ExecSignal sig = exec_stmt(flow_registry_body(flow));
    (void)sig;

    free(flow);

    Value outv = g_has_return ? g_return_value : value_num(0, "int");

    pop_scope();

    g_return_value = prev_ret;
    g_has_return = prev_has;

    g_call_depth--;

    return outv;
}

static Value eval_flow_call_name(const char *callee, ExprList *args, int has_implicit, Value implicit) {
    FlowHandle *flow = find_flow_handle(callee);
    if (!flow) runtime_error("Call to undefined flow");
    return eval_flow_call_handle(flow, callee, args, has_implicit, implicit);
}

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

    if (e->kind == EXPR_FIELD) {
        Symbol *sym = get_symbol_or_error(e->base);
        if (strcmp(sym->type, "struct") != 0) runtime_error("Field access requires ensemble instance");
        StructInstance *inst = (StructInstance*)sym->ptr_value;
        if (!inst) runtime_error("Null ensemble instance");

        StructFieldValue *fv = find_struct_field(inst, e->field);
        if (!fv) runtime_error("Unknown field on ensemble instance");
        return fv->value;
    }

    if (e->kind == EXPR_DOTCALL) {
        /* If dot_base is a variable holding a struct/class instance => implicit-this method call.
           Else treat as a static/qualified call Type.member(...).
        */
        Symbol *sym = get_symbol_or_null(e->dot_base);
        if (sym) {
            if (strcmp(sym->type, "struct") != 0) runtime_error("Method call requires ensemble/symphony instance");

            Value thisv = symbol_to_value(sym);
            char buf[256];
            snprintf(buf, sizeof(buf), "%s.%s", thisv.struct_type, e->dot_member);
            return eval_flow_call_name(buf, e->dot_args, 1, thisv);
        }

        char buf[256];
        snprintf(buf, sizeof(buf), "%s.%s", e->dot_base, e->dot_member);
        return eval_flow_call_name(buf, e->dot_args, 0, value_num(0, "int"));
    }

    if (e->kind == EXPR_CALL) {
        FlowHandle *flow = find_flow_handle(e->callee);
        if (!flow) {
            /* Constructor-like call: TypeName() creates an ensemble instance. */
            if (is_ensemble_type(e->callee)) {
                EnsembleDef *def = find_ensemble_def(e->callee);
                StructInstance *inst = make_struct_instance(e->callee);

                int argc = exprlist_length(e->args);
                int fieldc = fieldlist_length(def ? def->fields : NULL);
                if (argc != fieldc) {
                    if (argc == 0 && fieldc == 0) {
                        return value_struct(e->callee, inst);
                    }
                    runtime_error("Constructor argument count mismatch");
                }

                /* Initialize fields in definition order. */
                StructFieldValue *fv = inst->fields;
                FieldList *fd = def ? def->fields : NULL;
                ExprList *a = e->args;
                while (fv && fd && a) {
                    Value av = eval_expr(a->expr);
                    fv->value = coerce_to_field_type(av, fd->type);
                    fv = fv->next;
                    fd = fd->next;
                    a = a->next;
                }

                return value_struct(e->callee, inst);
            }
            runtime_error("Call to undefined flow");
        }

        return eval_flow_call_handle(flow, e->callee, e->args, 0, value_num(0, "int"));
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

    if (strcmp(v.type, "struct") == 0) {
        StructInstance *inst = (StructInstance*)v.ptr;
        fprintf(out, "%s{", v.struct_type);

        int first = 1;
        for (StructFieldValue *f = inst ? inst->fields : NULL; f; f = f->next) {
            if (!first) fprintf(out, ",");
            first = 0;

            fprintf(out, "%s=", f->name);
            Value fv = f->value;
            if (strcmp(fv.type, "int") == 0) fprintf(out, "%lld", (long long)fv.num);
            else if (strcmp(fv.type, "float") == 0) fprintf(out, "%g", fv.num);
            else if (strcmp(fv.type, "bool") == 0) fprintf(out, "%s", fv.boolean ? "true" : "false");
            else if (strcmp(fv.type, "string") == 0) fprintf(out, "%s", fv.str);
            else fprintf(out, "<%s>", fv.type);
        }

        fprintf(out, "}\n");
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
        case STMT_FIELD_STAGE: {
            Symbol *sym = get_symbol_or_error(s->base);
            if (strcmp(sym->type, "struct") != 0) runtime_error("Field assignment requires ensemble instance");
            StructInstance *inst = (StructInstance*)sym->ptr_value;
            if (!inst) runtime_error("Null ensemble instance");

            EnsembleDef *def = find_ensemble_def(inst->type_name);
            if (!def) runtime_error("Unknown ensemble type for instance");

            const char *field_type = NULL;
            for (FieldList *f = def->fields; f; f = f->next) {
                if (strcmp(f->name, s->field) == 0) { field_type = f->type; break; }
            }
            if (!field_type) runtime_error("Unknown field on ensemble instance");

            Value rhs = eval_expr(s->expr);
            Value coerced = coerce_to_field_type(rhs, field_type);

            StructFieldValue *fv = find_struct_field(inst, s->field);
            if (!fv) runtime_error("Unknown field on ensemble instance");
            fv->value = coerced;
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
    (void)e;
}

static void free_stmt_list(StmtList *list) {
    (void)list;
}

void free_stmt(Stmt *s) {
    (void)s;
}

void free_all_flows(void) {
    flow_registry_free_all();
}
