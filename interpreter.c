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

typedef struct ArrayInstance {
    int capacity;
    int length;
    Value *items;
} ArrayInstance;
typedef struct MapEntry {
    char *key;
    Value value;
    struct MapEntry *next;
} MapEntry;

typedef struct MapInstance {
    MapEntry *head;
    int size;
} MapInstance;

typedef struct SetEntry {
    char *key;
    struct SetEntry *next;
} SetEntry;

typedef struct SetInstance {
    SetEntry *head;
    int size;
} SetInstance;

static Value default_array_value(void);
static void array_reserve(ArrayInstance *arr, int new_cap);
static void array_push(ArrayInstance *arr, Value v);
static Value array_pop(ArrayInstance *arr);
static void array_resize(ArrayInstance *arr, int new_len);

static MapInstance* map_new(void);
static int map_has(MapInstance *m, const char *key);
static Value map_get(MapInstance *m, const char *key);
static void map_put(MapInstance *m, const char *key, Value v);
static int map_del(MapInstance *m, const char *key);

static SetInstance* set_new(void);
static int set_has(SetInstance *s, const char *key);
static void set_add(SetInstance *s, const char *key);
static int set_del(SetInstance *s, const char *key);

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

static Value value_map(void *ptr) {
    Value v;
    strcpy(v.type, "map");
    v.num = 0;
    v.str[0] = '\0';
    v.boolean = 0;
    v.ptr = ptr;
    v.struct_type[0] = '\0';
    return v;
}

static Value value_set(void *ptr) {
    Value v;
    strcpy(v.type, "set");
    v.num = 0;
    v.str[0] = '\0';
    v.boolean = 0;
    v.ptr = ptr;
    v.struct_type[0] = '\0';
    return v;
}

static Value value_array(void *ptr) {
    Value v;
    strcpy(v.type, "array");
    v.num = 0;
    v.str[0] = '\0';
    v.boolean = 0;
    v.ptr = ptr;
    v.struct_type[0] = '\0';
    return v;
}

static Value default_array_value(void) {
    return value_num(0, "int");
}

static void array_reserve(ArrayInstance *arr, int new_cap) {
    if (!arr) runtime_error("Null array instance");
    if (new_cap <= arr->capacity) return;
    if (new_cap < 1) new_cap = 1;

    Value *new_items = (Value*)arena_calloc((size_t)new_cap, sizeof(Value));
    if (!new_items) exit(1);

    for (int i = 0; i < arr->length; i++) {
        new_items[i] = arr->items[i];
    }
    for (int i = arr->length; i < new_cap; i++) {
        new_items[i] = default_array_value();
    }

    arr->items = new_items;
    arr->capacity = new_cap;
}

static void array_push(ArrayInstance *arr, Value v) {
    if (!arr) runtime_error("Null array instance");
    if (arr->length >= arr->capacity) {
        int next_cap = arr->capacity > 0 ? (arr->capacity * 2) : 1;
        array_reserve(arr, next_cap);
    }
    arr->items[arr->length++] = v;
}

static Value array_pop(ArrayInstance *arr) {
    if (!arr) runtime_error("Null array instance");
    if (arr->length <= 0) runtime_error("pop() on empty array");
    Value v = arr->items[arr->length - 1];
    arr->length--;
    return v;
}

static void array_resize(ArrayInstance *arr, int new_len) {
    if (!arr) runtime_error("Null array instance");
    if (new_len < 0) runtime_error("resize() requires newSize >= 0");

    if (new_len > arr->capacity) {
        int next_cap = arr->capacity > 0 ? arr->capacity : 1;
        while (next_cap < new_len) {
            if (next_cap > 100000000 / 2) { next_cap = new_len; break; }
            next_cap *= 2;
        }
        array_reserve(arr, next_cap);
    }

    if (new_len > arr->length) {
        for (int i = arr->length; i < new_len; i++) {
            arr->items[i] = default_array_value();
        }
    }
    arr->length = new_len;
}

/* -------------------------
   Ensemble (struct) registry + instances
   ------------------------- */

typedef struct EnsembleDef {
    char *name;
    char *parent;
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

static FieldList* copy_fieldlist(FieldList *src) {
    FieldList *out = NULL;
    FieldList *tail = NULL;
    for (FieldList *p = src; p; p = p->next) {
        FieldList *node = (FieldList*)arena_calloc(1, sizeof(FieldList));
        if (!node) exit(1);
        node->type = p->type;
        node->name = p->name;
        node->next = NULL;
        if (!out) { out = node; tail = node; }
        else { tail->next = node; tail = node; }
    }
    return out;
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

void register_ensemble(char *name, char *parent, FieldList *fields) {
    if (!name) runtime_error("Null type name");
    if (find_ensemble_def(name)) runtime_error("Duplicate type name");

    FieldList *final_fields = fields;
    if (parent) {
        EnsembleDef *pdef = find_ensemble_def(parent);
        if (!pdef) runtime_error("Unknown parent type in extends");
        FieldList *parent_copy = copy_fieldlist(pdef->fields);
        if (!parent_copy) {
            final_fields = fields;
        } else {
            FieldList *tail = parent_copy;
            while (tail->next) tail = tail->next;
            tail->next = fields;
            final_fields = parent_copy;
        }
    }

    EnsembleDef *node = (EnsembleDef*)arena_calloc(1, sizeof(EnsembleDef));
    if (!node) exit(1);
    node->name = name;
    node->parent = parent;
    node->fields = final_fields;
    node->next = g_ensembles;
    g_ensembles = node;
}

int is_ensemble_type(const char *name) {
    return find_ensemble_def(name) ? 1 : 0;
}

const char* ensemble_parent_or_null(const char *type_name) {
    EnsembleDef *def = find_ensemble_def(type_name);
    return def ? def->parent : NULL;
}

FieldList* ensemble_fields_or_null(const char *type_name) {
    EnsembleDef *def = find_ensemble_def(type_name);
    return def ? def->fields : NULL;
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

Expr* make_supercall(char *member, ExprList *args) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_SUPERCALL;
    e->super_member = member;
    e->super_args = args;
    return e;
}

Expr* make_superctor(ExprList *args) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_SUPERCTOR;
    e->super_ctor_args = args;
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

Stmt* make_score(Stmt *init, Expr *cond, Stmt *step, Stmt *body) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_SCORE;
    s->init = init;
    s->cond = cond;
    s->step = step;
    s->body = body;
    return s;
}

Stmt* make_fixed(char *name, Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_FIXED;
    s->name = name;
    s->expr = expr;
    return s;
}

Stmt* make_play(ExprList *args) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_PLAY;
    s->play_args = args;
    return s;
}

Stmt* make_return(Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_RETURN;
    s->expr = expr;
    return s;
}

Expr* make_addrof(char *varname) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_ADDROF;
    e->name = varname;
    return e;
}

Expr* make_deref(Expr *operand) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_DEREF;
    e->left = operand;
    return e;
}

Stmt* make_stagethru(char *ptrname, Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_STAGETHRU;
    s->name = ptrname;
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
    if (strcmp(sym->type, "array") == 0) return value_array(sym->ptr_value);
    if (strcmp(sym->type, "map") == 0) return value_map(sym->ptr_value);
    if (strcmp(sym->type, "set") == 0) return value_set(sym->ptr_value);
    if (strcmp(sym->type, "pointer") == 0) {
        Value v;
        memset(&v, 0, sizeof(v));
        strcpy(v.type, "pointer");
        strncpy(v.str, sym->str_value, sizeof(v.str) - 1);
        return v;
    }
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
    else if (strcmp(v.type, "array") == 0)
        declare_or_update_current_scope_array(name, v.ptr);
    else if (strcmp(v.type, "map") == 0)
        declare_or_update_current_scope_map(name, v.ptr);
    else if (strcmp(v.type, "set") == 0)
        declare_or_update_current_scope_set(name, v.ptr);
    else if (strcmp(v.type, "pointer") == 0)
        declare_or_update_current_scope_value(name, "pointer", 0, v.str, 0);
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
    else if (strcmp(v.type, "array") == 0)
        insert_or_update_array(name, v.ptr);
    else if (strcmp(v.type, "map") == 0)
        insert_or_update_map(name, v.ptr);
    else if (strcmp(v.type, "set") == 0)
        insert_or_update_set(name, v.ptr);
    else if (strcmp(v.type, "pointer") == 0)
        insert_or_update_value(name, "pointer", 0, v.str, 0);
    else
        runtime_error("Unsupported value type in stage");
}

static void store_value_fixed(const char *name, Value v) {
    /* Only scalar types are supported for fixed constants */
    if (strcmp(v.type, "int") == 0 || strcmp(v.type, "float") == 0)
        declare_const_value(name, v.type, v.num, NULL, 0);
    else if (strcmp(v.type, "bool") == 0)
        declare_const_value(name, "bool", 0, NULL, v.boolean);
    else if (strcmp(v.type, "string") == 0)
        declare_const_value(name, "string", 0, v.str, 0);
    else
        runtime_error("fixed only supports scalar constant values (int, float, bool, string)");
}

static long long value_to_index(Value v) {
    if (!is_numeric_type(v.type)) runtime_error("Array index must be numeric");
    long long i = (long long)v.num;
    if ((double)i != v.num) runtime_error("Array index must be an integer");
    return i;
}

static const char* value_to_key_string(const Value *v) {
    if (!v) runtime_error("Null key value");
    if (strcmp(v->type, "string") != 0) runtime_error("Map key must be a string");
    return v->str;
}

static MapInstance* map_new(void) {
    MapInstance *m = (MapInstance*)arena_calloc(1, sizeof(MapInstance));
    if (!m) exit(1);
    m->head = NULL;
    m->size = 0;
    return m;
}

static MapEntry* map_find_entry(MapInstance *m, const char *key) {
    for (MapEntry *e = m ? m->head : NULL; e; e = e->next) {
        if (strcmp(e->key, key) == 0) return e;
    }
    return NULL;
}

static int map_has(MapInstance *m, const char *key) {
    if (!m) runtime_error("Null map instance");
    if (!key) runtime_error("Null map key");
    return map_find_entry(m, key) ? 1 : 0;
}

static Value map_get(MapInstance *m, const char *key) {
    if (!m) runtime_error("Null map instance");
    if (!key) runtime_error("Null map key");
    MapEntry *e = map_find_entry(m, key);
    if (!e) runtime_error("Key not found in map");
    return e->value;
}

static void map_put(MapInstance *m, const char *key, Value v) {
    if (!m) runtime_error("Null map instance");
    if (!key) runtime_error("Null map key");
    MapEntry *e = map_find_entry(m, key);
    if (e) {
        e->value = v;
        return;
    }

    MapEntry *ne = (MapEntry*)arena_calloc(1, sizeof(MapEntry));
    if (!ne) exit(1);
    ne->key = arena_strdup(key);
    ne->value = v;
    ne->next = m->head;
    m->head = ne;
    m->size++;
}

static int map_del(MapInstance *m, const char *key) {
    if (!m) runtime_error("Null map instance");
    if (!key) runtime_error("Null map key");

    MapEntry *prev = NULL;
    for (MapEntry *e = m->head; e; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            if (prev) prev->next = e->next;
            else m->head = e->next;
            m->size--;
            return 1;
        }
        prev = e;
    }
    return 0;
}

static SetInstance* set_new(void) {
    SetInstance *s = (SetInstance*)arena_calloc(1, sizeof(SetInstance));
    if (!s) exit(1);
    s->head = NULL;
    s->size = 0;
    return s;
}

static SetEntry* set_find_entry(SetInstance *s, const char *key) {
    for (SetEntry *e = s ? s->head : NULL; e; e = e->next) {
        if (strcmp(e->key, key) == 0) return e;
    }
    return NULL;
}

static int set_has(SetInstance *s, const char *key) {
    if (!s) runtime_error("Null set instance");
    if (!key) runtime_error("Null set key");
    return set_find_entry(s, key) ? 1 : 0;
}

static void set_add(SetInstance *s, const char *key) {
    if (!s) runtime_error("Null set instance");
    if (!key) runtime_error("Null set key");
    if (set_find_entry(s, key)) return;

    SetEntry *ne = (SetEntry*)arena_calloc(1, sizeof(SetEntry));
    if (!ne) exit(1);
    ne->key = arena_strdup(key);
    ne->next = s->head;
    s->head = ne;
    s->size++;
}

static int set_del(SetInstance *s, const char *key) {
    if (!s) runtime_error("Null set instance");
    if (!key) runtime_error("Null set key");

    SetEntry *prev = NULL;
    for (SetEntry *e = s->head; e; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            if (prev) prev->next = e->next;
            else s->head = e->next;
            s->size--;
            return 1;
        }
        prev = e;
    }
    return 0;
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

Expr* make_arraylit(ExprList *elems) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_ARRAYLIT;
    e->array_elems = elems;
    return e;
}

Expr* make_index(Expr *target, Expr *index) {
    Expr *e = (Expr*)arena_calloc(1, sizeof(Expr));
    if (!e) exit(1);
    e->kind = EXPR_INDEX;
    e->index_target = target;
    e->index_expr = index;
    return e;
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
    else if (op == OP_DIV) {
        if (y == 0.0) runtime_error("Division by zero");
        r = x / y;
    }

    if (result_is_float) return value_num(r, "float");
    return value_num((double)((long long)r), "int");
}

Stmt* make_index_stage(char *name, Expr *index, Expr *expr) {
    Stmt *s = (Stmt*)arena_calloc(1, sizeof(Stmt));
    if (!s) exit(1);
    s->kind = STMT_INDEX_STAGE;
    s->name = name;
    s->index = index;
    s->expr = expr;
    return s;
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

static char* resolve_method_callee(const char *start_type, const char *member) {
    const char *cur = start_type;
    while (cur && cur[0]) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s.%s", cur, member);
        FlowHandle *h = find_flow_handle(buf);
        if (h) {
            free(h);
            return arena_strdup(buf);
        }

        EnsembleDef *def = find_ensemble_def(cur);
        cur = def ? def->parent : NULL;
    }
    return NULL;
}

static char* resolve_super_method_callee(const char *child_type, const char *member) {
    EnsembleDef *def = find_ensemble_def(child_type);
    const char *parent = def ? def->parent : NULL;
    if (!parent) return NULL;
    return resolve_method_callee(parent, member);
}

static Value get_this_value_or_error(void) {
    Symbol *this_sym = get_symbol_or_null("this");
    if (!this_sym || strcmp(this_sym->type, "struct") != 0) runtime_error("this is not available here");
    return symbol_to_value(this_sym);
}

static void init_parent_fields_fallback(StructInstance *inst, const char *parent_type, ExprList *args) {
    EnsembleDef *pdef = find_ensemble_def(parent_type);
    if (!pdef) runtime_error("Unknown parent type");

    int argc = exprlist_length(args);
    int fieldc = fieldlist_length(pdef->fields);
    if (argc != fieldc) runtime_error("super() argument count mismatch");

    StructFieldValue *fv = inst ? inst->fields : NULL;
    FieldList *fd = pdef->fields;
    ExprList *a = args;
    while (fv && fd && a) {
        Value av = eval_expr(a->expr);
        fv->value = coerce_to_field_type(av, fd->type);
        fv = fv->next;
        fd = fd->next;
        a = a->next;
    }
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
            char *callee = resolve_method_callee(thisv.struct_type, e->dot_member);
            if (!callee) runtime_error("Undefined method for type");
            return eval_flow_call_name(callee, e->dot_args, 1, thisv);
        }

        /* Static call: Type.method(...) with inheritance lookup */
        char *callee = resolve_method_callee(e->dot_base, e->dot_member);
        if (!callee) runtime_error("Call to undefined flow");
        return eval_flow_call_name(callee, e->dot_args, 0, value_num(0, "int"));
    }

    if (e->kind == EXPR_SUPERCALL) {
        /* Only valid inside an implicit-this method call (requires `this` bound). */
        Symbol *this_sym = get_symbol_or_null("this");
        if (!this_sym || strcmp(this_sym->type, "struct") != 0) runtime_error("super.method() requires a method context with this");

        Value thisv = symbol_to_value(this_sym);
        char *callee = resolve_super_method_callee(thisv.struct_type, e->super_member);
        if (!callee) runtime_error("Undefined super method");
        return eval_flow_call_name(callee, e->super_args, 1, thisv);
    }

    if (e->kind == EXPR_SUPERCTOR) {
        Value thisv = get_this_value_or_error();

        EnsembleDef *cdef = find_ensemble_def(thisv.struct_type);
        const char *parent = cdef ? cdef->parent : NULL;
        if (!parent) runtime_error("super() used but type has no parent");

        /* Prefer calling Parent.init(args) if it exists; otherwise initialize parent fields directly. */
        char buf[256];
        snprintf(buf, sizeof(buf), "%s.init", parent);
        FlowHandle *h = find_flow_handle(buf);
        if (h) {
            return eval_flow_call_handle(h, buf, e->super_ctor_args, 1, thisv);
        }

        init_parent_fields_fallback((StructInstance*)thisv.ptr, parent, e->super_ctor_args);
        return value_num(0, "int");
    }

    if (e->kind == EXPR_ARRAYLIT) {
        int n = exprlist_length(e->array_elems);
        ArrayInstance *arr = (ArrayInstance*)arena_calloc(1, sizeof(ArrayInstance));
        if (!arr) exit(1);
        arr->capacity = n;
        arr->length = n;
        arr->items = n > 0 ? (Value*)arena_calloc((size_t)n, sizeof(Value)) : NULL;

        int i = 0;
        for (ExprList *p = e->array_elems; p; p = p->next) {
            arr->items[i++] = eval_expr(p->expr);
        }
        return value_array(arr);
    }

    if (e->kind == EXPR_INDEX) {
        Value tv = eval_expr(e->index_target);
        Value iv = eval_expr(e->index_expr);
        if (strcmp(tv.type, "array") == 0) {
            ArrayInstance *arr = (ArrayInstance*)tv.ptr;
            if (!arr) runtime_error("Null array instance");
            long long idx = value_to_index(iv);
            if (idx < 0 || idx >= (long long)arr->length) runtime_error("Array index out of bounds");
            return arr->items[(int)idx];
        }

        if (strcmp(tv.type, "map") == 0) {
            MapInstance *m = (MapInstance*)tv.ptr;
            if (!m) runtime_error("Null map instance");
            const char *key = value_to_key_string(&iv);
            return map_get(m, key);
        }

        if (strcmp(tv.type, "set") == 0) {
            SetInstance *s = (SetInstance*)tv.ptr;
            if (!s) runtime_error("Null set instance");
            const char *key = value_to_key_string(&iv);
            return value_bool(set_has(s, key));
        }

        runtime_error("Indexing requires an array, map, or set");
    }

    if (e->kind == EXPR_ADDROF) {
        /* Return a pointer value: type="pointer", str = variable name */
        if (!e->name) runtime_error("addrof: null variable name");
        Value v;
        memset(&v, 0, sizeof(v));
        strcpy(v.type, "pointer");
        strncpy(v.str, e->name, sizeof(v.str) - 1);
        return v;
    }

    if (e->kind == EXPR_DEREF) {
        Value pv = eval_expr(e->left);
        if (strcmp(pv.type, "pointer") != 0) runtime_error("deref: value is not a pointer");
        Symbol *sym = get_symbol_or_error(pv.str);
        return symbol_to_value(sym);
    }

    if (e->kind == EXPR_CALL) {
        /* Built-in array helpers (runtime intrinsics) */
        if (e->callee && strcmp(e->callee, "array") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 1) runtime_error("array(n) expects exactly 1 argument");
            Value nval = eval_expr(e->args->expr);
            long long nll = value_to_index(nval);
            if (nll < 0) runtime_error("array(n) requires n >= 0");
            if (nll > 1000000) runtime_error("array(n) too large");

            int n = (int)nll;
            ArrayInstance *arr = (ArrayInstance*)arena_calloc(1, sizeof(ArrayInstance));
            if (!arr) exit(1);
            arr->capacity = n;
            arr->length = n;
            arr->items = n > 0 ? (Value*)arena_calloc((size_t)n, sizeof(Value)) : NULL;
            for (int i = 0; i < n; i++) {
                arr->items[i] = default_array_value();
            }
            return value_array(arr);
        }

        if (e->callee && strcmp(e->callee, "push") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 2) runtime_error("push(arr, value) expects exactly 2 arguments");
            Value av = eval_expr(e->args->expr);
            if (strcmp(av.type, "array") != 0) runtime_error("push() first argument must be an array");
            Value vv = eval_expr(e->args->next->expr);
            array_push((ArrayInstance*)av.ptr, vv);
            return value_num((double)((ArrayInstance*)av.ptr)->length, "int");
        }

        if (e->callee && strcmp(e->callee, "pop") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 1) runtime_error("pop(arr) expects exactly 1 argument");
            Value av = eval_expr(e->args->expr);
            if (strcmp(av.type, "array") != 0) runtime_error("pop() argument must be an array");
            return array_pop((ArrayInstance*)av.ptr);
        }

        if (e->callee && strcmp(e->callee, "resize") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 2) runtime_error("resize(arr, newSize) expects exactly 2 arguments");
            Value av = eval_expr(e->args->expr);
            if (strcmp(av.type, "array") != 0) runtime_error("resize() first argument must be an array");
            Value nv = eval_expr(e->args->next->expr);
            long long nll = value_to_index(nv);
            if (nll < 0) runtime_error("resize() requires newSize >= 0");
            if (nll > 1000000) runtime_error("resize() too large");

            array_resize((ArrayInstance*)av.ptr, (int)nll);
            return value_num((double)((ArrayInstance*)av.ptr)->length, "int");
        }

        /* Built-in map helpers (string key -> any value) */
        if (e->callee && strcmp(e->callee, "map") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 0) runtime_error("map() expects no arguments");
            return value_map(map_new());
        }

        /* Built-in set helpers (string elements) */
        if (e->callee && strcmp(e->callee, "set") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 0) runtime_error("set() expects no arguments");
            return value_set(set_new());
        }

        if (e->callee && strcmp(e->callee, "add") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 2) runtime_error("add(s, key) expects exactly 2 arguments");
            Value sv = eval_expr(e->args->expr);
            if (strcmp(sv.type, "set") != 0) runtime_error("add() first argument must be a set");
            Value kv = eval_expr(e->args->next->expr);
            const char *key = value_to_key_string(&kv);
            set_add((SetInstance*)sv.ptr, key);
            return value_num((double)((SetInstance*)sv.ptr)->size, "int");
        }

        if (e->callee && strcmp(e->callee, "has") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 2) runtime_error("has(m, key) expects exactly 2 arguments");
            Value cv = eval_expr(e->args->expr);
            Value kv = eval_expr(e->args->next->expr);
            const char *key = value_to_key_string(&kv);
            if (strcmp(cv.type, "map") == 0) return value_bool(map_has((MapInstance*)cv.ptr, key));
            if (strcmp(cv.type, "set") == 0) return value_bool(set_has((SetInstance*)cv.ptr, key));
            runtime_error("has() first argument must be a map or set");
        }

        if (e->callee && strcmp(e->callee, "get") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 2) runtime_error("get(m, key) expects exactly 2 arguments");
            Value mv = eval_expr(e->args->expr);
            if (strcmp(mv.type, "map") != 0) runtime_error("get() first argument must be a map");
            Value kv = eval_expr(e->args->next->expr);
            const char *key = value_to_key_string(&kv);
            return map_get((MapInstance*)mv.ptr, key);
        }

        if (e->callee && strcmp(e->callee, "put") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 3) runtime_error("put(m, key, value) expects exactly 3 arguments");
            Value mv = eval_expr(e->args->expr);
            if (strcmp(mv.type, "map") != 0) runtime_error("put() first argument must be a map");
            Value kv = eval_expr(e->args->next->expr);
            const char *key = value_to_key_string(&kv);
            Value vv = eval_expr(e->args->next->next->expr);
            map_put((MapInstance*)mv.ptr, key, vv);
            return vv;
        }

        if (e->callee && strcmp(e->callee, "del") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 2) runtime_error("del(m, key) expects exactly 2 arguments");
            Value cv = eval_expr(e->args->expr);
            Value kv = eval_expr(e->args->next->expr);
            const char *key = value_to_key_string(&kv);
            if (strcmp(cv.type, "map") == 0) return value_bool(map_del((MapInstance*)cv.ptr, key));
            if (strcmp(cv.type, "set") == 0) return value_bool(set_del((SetInstance*)cv.ptr, key));
            runtime_error("del() first argument must be a map or set");
        }

        if (e->callee && strcmp(e->callee, "keys") == 0) {
            int argc = exprlist_length(e->args);
            if (argc != 1) runtime_error("keys(m) expects exactly 1 argument");
            Value mv = eval_expr(e->args->expr);
            int n = 0;
            if (strcmp(mv.type, "map") == 0) {
                MapInstance *m = (MapInstance*)mv.ptr;
                if (!m) runtime_error("Null map instance");
                n = m->size;

                ArrayInstance *arr = (ArrayInstance*)arena_calloc(1, sizeof(ArrayInstance));
                if (!arr) exit(1);
                arr->capacity = n;
                arr->length = n;
                arr->items = arr->length > 0 ? (Value*)arena_calloc((size_t)arr->length, sizeof(Value)) : NULL;

                int i = 0;
                for (MapEntry *e2 = m->head; e2; e2 = e2->next) {
                    arr->items[i++] = value_string(e2->key);
                }
                return value_array(arr);
            }

            if (strcmp(mv.type, "set") == 0) {
                SetInstance *s = (SetInstance*)mv.ptr;
                if (!s) runtime_error("Null set instance");
                n = s->size;

                ArrayInstance *arr = (ArrayInstance*)arena_calloc(1, sizeof(ArrayInstance));
                if (!arr) exit(1);
                arr->capacity = n;
                arr->length = n;
                arr->items = arr->length > 0 ? (Value*)arena_calloc((size_t)arr->length, sizeof(Value)) : NULL;

                int i = 0;
                for (SetEntry *e2 = s->head; e2; e2 = e2->next) {
                    arr->items[i++] = value_string(e2->key);
                }
                return value_array(arr);
            }

            runtime_error("keys() argument must be a map or set");
        }

        FlowHandle *flow = find_flow_handle(e->callee);
        if (!flow) {
            /* Constructor-like call: TypeName() creates an ensemble instance. */
            if (is_ensemble_type(e->callee)) {
                EnsembleDef *def = find_ensemble_def(e->callee);
                StructInstance *inst = make_struct_instance(e->callee);

                /* If there is a user-defined init method, prefer it as the constructor. */
                {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "%s.init", e->callee);
                    FlowHandle *ih = find_flow_handle(buf);
                    if (ih) {
                        Value thisv = value_struct(e->callee, inst);
                        (void)eval_flow_call_handle(ih, buf, e->args, 1, thisv);
                        return value_struct(e->callee, inst);
                    }
                }

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

static void fprint_value_inline(FILE *out, Value v) {
    if (strcmp(v.type, "int") == 0) {
        fprintf(out, "%lld", (long long)v.num);
        return;
    }
    if (strcmp(v.type, "float") == 0) {
        fprintf(out, "%g", v.num);
        return;
    }
    if (strcmp(v.type, "bool") == 0) {
        fprintf(out, "%s", v.boolean ? "true" : "false");
        return;
    }
    if (strcmp(v.type, "string") == 0) {
        fprintf(out, "%s", v.str);
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
            fprint_value_inline(out, f->value);
        }

        fprintf(out, "}");
        return;
    }

    if (strcmp(v.type, "array") == 0) {
        ArrayInstance *arr = (ArrayInstance*)v.ptr;
        fprintf(out, "[");
        for (int i = 0; i < (arr ? arr->length : 0); i++) {
            if (i) fprintf(out, ",");
            fprint_value_inline(out, arr->items[i]);
        }
        fprintf(out, "]");
        return;
    }

    if (strcmp(v.type, "map") == 0) {
        MapInstance *m = (MapInstance*)v.ptr;
        fprintf(out, "{");
        int first = 1;
        for (MapEntry *e = m ? m->head : NULL; e; e = e->next) {
            if (!first) fprintf(out, ",");
            first = 0;
            fprintf(out, "%s=", e->key);
            fprint_value_inline(out, e->value);
        }
        fprintf(out, "}");
        return;
    }

    if (strcmp(v.type, "set") == 0) {
        SetInstance *s = (SetInstance*)v.ptr;
        fprintf(out, "set{");
        int first = 1;
        for (SetEntry *e = s ? s->head : NULL; e; e = e->next) {
            if (!first) fprintf(out, ",");
            first = 0;
            fprintf(out, "%s", e->key);
        }
        fprintf(out, "}");
        return;
    }

    fprintf(out, "<%s>", v.type);
}

static void print_value(Value v) {
    FILE *out = g_out ? g_out : stdout;
    fprint_value_inline(out, v);
    fprintf(out, "\n");
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
        case STMT_INDEX_STAGE: {
            Symbol *sym = get_symbol_or_error(s->name);
            Value iv = eval_expr(s->index);
            Value rhs = eval_expr(s->expr);
            if (strcmp(sym->type, "array") == 0) {
                ArrayInstance *arr = (ArrayInstance*)sym->ptr_value;
                if (!arr) runtime_error("Null array instance");

                long long idx = value_to_index(iv);
                if (idx < 0 || idx >= (long long)arr->length) runtime_error("Array index out of bounds");
                arr->items[(int)idx] = rhs;
                return SIG_OK;
            }

            if (strcmp(sym->type, "map") == 0) {
                MapInstance *m = (MapInstance*)sym->ptr_value;
                if (!m) runtime_error("Null map instance");
                const char *key = value_to_key_string(&iv);
                map_put(m, key, rhs);
                return SIG_OK;
            }

            if (strcmp(sym->type, "set") == 0) {
                SetInstance *s = (SetInstance*)sym->ptr_value;
                if (!s) runtime_error("Null set instance");
                const char *key = value_to_key_string(&iv);
                if (strcmp(rhs.type, "bool") != 0) runtime_error("Set assignment requires bool (true adds, false removes)");
                if (rhs.boolean) set_add(s, key);
                else (void)set_del(s, key);
                return SIG_OK;
            }

            runtime_error("Index assignment requires array, map, or set variable");
            return SIG_OK;
        }
        case STMT_STAGETHRU: {
            /* Write through pointer: look up the pointer var, find its target, update it. */
            Symbol *psym = get_symbol_or_error(s->name);
            if (strcmp(psym->type, "pointer") != 0) runtime_error("stagethru: variable is not a pointer");
            Value rhs = eval_expr(s->expr);
            /* str_value of the pointer symbol holds the target variable name */
            const char *target = psym->str_value;
            if (strcmp(rhs.type, "string") == 0)
                insert_or_update_value(target, rhs.type, rhs.num, rhs.str, rhs.boolean);
            else
                insert_or_update_value(target, rhs.type, rhs.num, NULL, rhs.boolean);
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
            long long iters = 0;
            const long long max_iters = 100000;
            for (;;) {
                if (++iters > max_iters) {
                    runtime_error("Execution limit exceeded (possible infinite loop)");
                }
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
        case STMT_SCORE: {
            /* for-loop: score (init; cond; step) { body } */
            g_loop_depth++;
            push_scope(); /* scope for the init variable */
            if (s->init) exec_stmt(s->init);
            long long iters = 0;
            const long long max_iters = 100000;
            for (;;) {
                if (++iters > max_iters) {
                    pop_scope();
                    runtime_error("Execution limit exceeded (possible infinite loop)");
                }
                if (s->cond) {
                    Value c = eval_expr(s->cond);
                    if (strcmp(c.type, "bool") != 0) runtime_error("score condition must be bool");
                    if (!c.boolean) break;
                }
                ExecSignal sig = exec_stmt(s->body);
                if (sig == SIG_BREAK) { break; }
                if (sig == SIG_RETURN) { pop_scope(); g_loop_depth--; return SIG_RETURN; }
                /* SIG_CONTINUE falls through to execute the step */
                if (s->step) exec_stmt(s->step);
            }
            pop_scope();
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
        case STMT_FIXED: {
            Value v = eval_expr(s->expr);
            store_value_fixed(s->name, v);
            return SIG_OK;
        }
        case STMT_PLAY: {
            ExprList *args = s->play_args;
            if (!args) return SIG_OK;
            FILE *out = g_out ? g_out : stdout;
            Value first = eval_expr(args->expr);
            if (!args->next) {
                /* Single arg — print without newline */
                fprint_value_inline(out, first);
                return SIG_OK;
            }
            /* Format string mode: first arg is format, rest are substitution values */
            if (strcmp(first.type, "string") != 0)
                runtime_error("play: first argument must be a format string when multiple arguments are given");
            const char *fmt = first.str;
            ExprList *cur = args->next;
            while (*fmt) {
                if (*fmt == '%' && *(fmt + 1) != '\0') {
                    fmt++;
                    char spec = *fmt++;
                    if (!cur) { fputc('%', out); fputc(spec, out); continue; }
                    Value av = eval_expr(cur->expr);
                    cur = cur->next;
                    switch (spec) {
                        case 'd': fprintf(out, "%lld", (long long)av.num); break;
                        case 'f': fprintf(out, "%g", av.num); break;
                        case 's': fprint_value_inline(out, av); break;
                        case 'b': fprintf(out, "%s", av.boolean ? "true" : "false"); break;
                        default: fputc('%', out); fputc(spec, out); break;
                    }
                } else {
                    fputc(*fmt++, out);
                }
            }
            return SIG_OK;
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
