#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

/* Forward declarations so Bison/Flex can use pointers in YYSTYPE. */
typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
typedef struct ExprList ExprList;
typedef struct NameList NameList;

/* Expression/statement kinds */
enum { EXPR_LIT = 1, EXPR_VAR, EXPR_BIN, EXPR_UNARY };
enum { EXPR_CALL = 5 };

enum { STMT_NOTE = 1, STMT_STAGE, STMT_EMIT, STMT_BLOCK, STMT_BRANCH, STMT_REPEAT, STMT_BREAK, STMT_CONTINUE, STMT_RETURN };

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

    /* binary */
    int op;
    Expr *left;
    Expr *right;

    /* call */
    char *callee;
    ExprList *args;
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

    /* block */
    StmtList *list;

    /* branch/repeat */
    Expr *cond;
    Stmt *then_block;
    Stmt *else_block;
    Stmt *body;
};

/* AST constructors used by the parser */
Expr* make_lit_num(double num, int is_float);
Expr* make_lit_bool(int boolean);
Expr* make_lit_string(const char *s);
Expr* make_var(char *name /* takes ownership */);
Expr* make_bin(int op, Expr *l, Expr *r);
Expr* make_unary(int op, Expr *operand);
Expr* make_call(char *callee /* takes ownership */, ExprList *args);

ExprList* exprlist_append(ExprList *list, Expr *expr);
NameList* namelist_append(NameList *list, char *name /* takes ownership */);

StmtList* stmtlist_append(StmtList *list, Stmt *stmt);

Stmt* make_block(StmtList *list);
Stmt* make_assign(int kind /* STMT_NOTE/STMT_STAGE/STMT_BREAK/STMT_CONTINUE */, char *name /* takes ownership (can be NULL) */, Expr *expr);
Stmt* make_emit(Expr *expr);
Stmt* make_branch(Expr *cond, Stmt *then_block, Stmt *else_block);
Stmt* make_repeat(Expr *cond, Stmt *body);
Stmt* make_return(Expr *expr);

/* Execute and free */
void execute_program(Stmt *root, FILE *out);
void free_stmt(Stmt *s);

/* Flow registry for function calls */
void register_flow(char *name /* takes ownership */, NameList *params /* takes ownership */, Stmt *body /* takes ownership */);
void free_all_flows(void);

#endif
