#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Simple arena allocator used for AST + identifier storage.
    This is a low-risk bridge step toward full C++ RAII.
    - Use arena_strdup() for IDENTIFIER token storage.
    - Call arena_free_all() once at end of program.
*/
char* arena_strdup(const char *s);
void arena_free_all(void);

/* Forward declarations so Bison/Flex can use pointers in YYSTYPE. */
typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
typedef struct ExprList ExprList;
typedef struct NameList NameList;
typedef struct ChainPart ChainPart;
typedef struct FieldList FieldList;

/* Expression/statement kinds */
enum { EXPR_LIT = 1, EXPR_VAR, EXPR_FIELD, EXPR_DOTCALL, EXPR_SUPERCALL, EXPR_SUPERCTOR, EXPR_BIN, EXPR_UNARY, EXPR_CALL, EXPR_CHAIN, EXPR_ARRAYLIT, EXPR_INDEX };

enum { STMT_NOTE = 1, STMT_STAGE, STMT_FIELD_STAGE, STMT_EMIT, STMT_BLOCK, STMT_BRANCH, STMT_REPEAT, STMT_BREAK, STMT_CONTINUE, STMT_RETURN, STMT_EXPR, STMT_INDEX_STAGE, STMT_SCORE };

/* Internal operator codes for the interpreter */
enum { OP_PLUS = 1, OP_MINUS, OP_MUL, OP_DIV, OP_LT, OP_LE, OP_GT, OP_GE, OP_EQ, OP_NE, OP_AND, OP_OR, OP_NOT, OP_NEG };

struct Expr {
    int kind;

    /* literal */
    char lit_type[16]; /* "int", "float", "string", "bool" */
    double lit_num;
    int lit_bool;
    char lit_str[256];

    /* variable */
    char *name;

    /* field access: base.field (base is a variable name for MVP) */
    char *base;
    char *field;

    /* dot call: base.member(args) where base may be a type name or a variable name */
    char *dot_base;
    char *dot_member;
    ExprList *dot_args;

    /* super call: super.member(args) (resolved relative to this.type's parent) */
    char *super_member;
    ExprList *super_args;

    /* super ctor call: super(args) (calls parent init) */
    ExprList *super_ctor_args;

    /* array literal: [a,b,c] */
    ExprList *array_elems;

    /* index: target[index] */
    Expr *index_target;
    Expr *index_expr;

    /* binary */
    int op;
    Expr *left;
    Expr *right;

    /* call */
    char *callee;
    ExprList *args;

    /* comparison chain */
    Expr *chain_first;
    ChainPart *chain_rest;
};

struct FieldList {
    char *type;   /* "int"/"float"/"bool"/"string" */
    char *name;   /* field name */
    FieldList *next;
};

struct ChainPart {
    int op; /* OP_LT/OP_LE/OP_GT/OP_GE */
    Expr *expr;
    ChainPart *next;
};

struct ExprList {
    Expr *expr;
    ExprList *next;
};

struct NameList {
    char *name;
    NameList *next;
};

struct StmtList {
    Stmt *stmt;
    StmtList *next;
};

struct Stmt {
    int kind;
    char *name; /* for NOTE/STAGE */
    Expr *expr; /* for NOTE/STAGE/EMIT */

    /* for field stage: base.field = expr */
    char *base;
    char *field;

    /* for index stage: name[index] = expr */
    Expr *index;

    /* block */
    StmtList *list;

    /* branch/repeat */
    Expr *cond;
    Stmt *then_block;
    Stmt *else_block;
    Stmt *body;

    /* score (for-loop): init; cond; step */
    Stmt *init;   /* NOTE or STAGE statement, or NULL */
    Stmt *step;   /* STAGE statement, or NULL */
};

/* AST constructors used by the parser */
Expr* make_lit_num(double num, int is_float);
Expr* make_lit_bool(int boolean);
Expr* make_lit_string(const char *s);
Expr* make_var(char *name /* takes ownership */);
Expr* make_field(char *base /* takes ownership */, char *field /* takes ownership */);
Expr* make_dotcall(char *base /* takes ownership */, char *member /* takes ownership */, ExprList *args);
Expr* make_supercall(char *member /* takes ownership */, ExprList *args);
Expr* make_superctor(ExprList *args);
Expr* make_arraylit(ExprList *elems);
Expr* make_index(Expr *target, Expr *index);
Expr* make_bin(int op, Expr *l, Expr *r);
Expr* make_unary(int op, Expr *operand);
Expr* make_call(char *callee /* takes ownership */, ExprList *args);
Expr* make_chain(Expr *first, ChainPart *rest);

ExprList* exprlist_append(ExprList *list, Expr *expr);
NameList* namelist_append(NameList *list, char *name /* takes ownership */);
ChainPart* chainpart_append(ChainPart *list, int op, Expr *expr);

StmtList* stmtlist_append(StmtList *list, Stmt *stmt);

Stmt* make_block(StmtList *list);
Stmt* make_assign(int kind /* STMT_NOTE/STMT_STAGE/STMT_BREAK/STMT_CONTINUE */, char *name /* takes ownership (can be NULL) */, Expr *expr);
Stmt* make_emit(Expr *expr);
Stmt* make_expr_stmt(Expr *expr);
Stmt* make_field_stage(char *base /* takes ownership */, char *field /* takes ownership */, Expr *expr);
Stmt* make_index_stage(char *name /* takes ownership */, Expr *index, Expr *expr);
Stmt* make_branch(Expr *cond, Stmt *then_block, Stmt *else_block);
Stmt* make_repeat(Expr *cond, Stmt *body);
Stmt* make_score(Stmt *init, Expr *cond, Stmt *step, Stmt *body);
Stmt* make_return(Expr *expr);

/* Ensemble (struct) registry */
FieldList* fieldlist_append(FieldList *list, char *type /* takes ownership */, char *name /* takes ownership */);
void register_ensemble(char *name /* takes ownership */, char *parent /* can be NULL, takes ownership */, FieldList *fields /* takes ownership */);
int is_ensemble_type(const char *name);

/* Metadata accessors (used by VM backend for Milestone 8). */
const char* ensemble_parent_or_null(const char *type_name);
FieldList* ensemble_fields_or_null(const char *type_name);

/* VM backend helpers: inspect registered ensemble metadata. Pointers are arena-owned. */
const char* ensemble_parent_or_null(const char *type_name);
FieldList* ensemble_fields_or_null(const char *type_name);

/* Execute and free */
void execute_program(Stmt *root, FILE *out);
void free_stmt(Stmt *s);

/* Flow registry for function calls */
void register_flow(char *name /* takes ownership */, NameList *params /* takes ownership */, Stmt *body /* takes ownership */);
void free_all_flows(void);

#ifdef __cplusplus
}
#endif

#endif
