%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern FILE *yyout;

int yylex();
int yyerror(const char *s);

#include "symbol_table.h"

/* ----- minimal interpreter types ----- */
typedef struct Value {
  char type[16];
  double num;
  char str[256];
  int boolean;
} Value;

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct StmtList StmtList;

struct Expr {
  int kind;
  Value lit;
  char *name;
  int op;
  Expr *left;
  Expr *right;
};

struct StmtList {
  Stmt *stmt;
  StmtList *next;
};

struct Stmt {
  int kind;
  char *name;
  Expr *expr;
  StmtList *list; /* for blocks */
  Expr *cond;
  Stmt *then_block;
  Stmt *else_block;
  Stmt *body;
};

enum { EXPR_LIT = 1, EXPR_VAR, EXPR_BIN };
enum { STMT_NOTE = 1, STMT_STAGE, STMT_EMIT, STMT_BLOCK, STMT_BRANCH, STMT_REPEAT };
enum { OP_PLUS = 1, OP_MINUS, OP_MUL, OP_DIV, OP_LT, OP_GT, OP_EQ, OP_AND, OP_OR };

static void runtime_error(const char *msg) {
  fprintf(yyout ? yyout : stdout, "Runtime Error: %s\n", msg);
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

static Expr* make_lit(Value v) {
  Expr *e = (Expr*)calloc(1, sizeof(Expr));
  if (!e) exit(1);
  e->kind = EXPR_LIT;
  e->lit = v;
  return e;
}

static Expr* make_var(char *name) {
  Expr *e = (Expr*)calloc(1, sizeof(Expr));
  if (!e) exit(1);
  e->kind = EXPR_VAR;
  e->name = name;
  return e;
}

static Expr* make_bin(int op, Expr *l, Expr *r) {
  Expr *e = (Expr*)calloc(1, sizeof(Expr));
  if (!e) exit(1);
  e->kind = EXPR_BIN;
  e->op = op;
  e->left = l;
  e->right = r;
  return e;
}

static StmtList* stmtlist_append(StmtList *list, Stmt *stmt) {
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

static Stmt* make_block(StmtList *list) {
  Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
  if (!s) exit(1);
  s->kind = STMT_BLOCK;
  s->list = list;
  return s;
}

static Stmt* make_assign(int kind, char *name, Expr *expr) {
  Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
  if (!s) exit(1);
  s->kind = kind;
  s->name = name;
  s->expr = expr;
  return s;
}

static Stmt* make_emit(Expr *expr) {
  Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
  if (!s) exit(1);
  s->kind = STMT_EMIT;
  s->expr = expr;
  return s;
}

static Stmt* make_branch(Expr *cond, Stmt *then_block, Stmt *else_block) {
  Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
  if (!s) exit(1);
  s->kind = STMT_BRANCH;
  s->cond = cond;
  s->then_block = then_block;
  s->else_block = else_block;
  return s;
}

static Stmt* make_repeat(Expr *cond, Stmt *body) {
  Stmt *s = (Stmt*)calloc(1, sizeof(Stmt));
  if (!s) exit(1);
  s->kind = STMT_REPEAT;
  s->cond = cond;
  s->body = body;
  return s;
}

static Value eval_expr(Expr *e);
static void exec_stmt(Stmt *s);

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

static Value eval_expr(Expr *e) {
  if (!e) runtime_error("Null expression");
  if (e->kind == EXPR_LIT) return e->lit;
  if (e->kind == EXPR_VAR) {
    Symbol *sym = get_symbol_or_error(e->name);
    return symbol_to_value(sym);
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
      } else {
        if (left.boolean) return value_bool(1);
        Value right = eval_expr(e->right);
        if (strcmp(right.type, "bool") != 0) runtime_error("Logical operators require bool");
        return value_bool(left.boolean || right.boolean);
      }
    }

    Value a = eval_expr(e->left);
    Value b = eval_expr(e->right);

    if (e->op == OP_PLUS || e->op == OP_MINUS || e->op == OP_MUL || e->op == OP_DIV) {
      return eval_bin_numeric(e->op, a, b);
    }

    if (e->op == OP_LT || e->op == OP_GT) {
      if (!is_numeric_type(a.type) || !is_numeric_type(b.type)) runtime_error("Comparison requires numeric types");
      if (e->op == OP_LT) return value_bool(a.num < b.num);
      return value_bool(a.num > b.num);
    }

    if (e->op == OP_EQ) {
      if (is_numeric_type(a.type) && is_numeric_type(b.type)) return value_bool(a.num == b.num);
      if (strcmp(a.type, "bool") == 0 && strcmp(b.type, "bool") == 0) return value_bool(a.boolean == b.boolean);
      if (strcmp(a.type, "string") == 0 && strcmp(b.type, "string") == 0) return value_bool(strcmp(a.str, b.str) == 0);
      runtime_error("== operands must be comparable");
    }
  }
  runtime_error("Unknown expression kind");
  return value_num(0, "int");
}

static void exec_stmt_list(StmtList *list) {
  for (StmtList *cur = list; cur; cur = cur->next) exec_stmt(cur->stmt);
}

static void print_value(Value v) {
  if (strcmp(v.type, "int") == 0) {
    fprintf(yyout ? yyout : stdout, "%lld\n", (long long)v.num);
    return;
  }
  if (strcmp(v.type, "float") == 0) {
    fprintf(yyout ? yyout : stdout, "%g\n", v.num);
    return;
  }
  if (strcmp(v.type, "bool") == 0) {
    fprintf(yyout ? yyout : stdout, "%s\n", v.boolean ? "true" : "false");
    return;
  }
  if (strcmp(v.type, "string") == 0) {
    fprintf(yyout ? yyout : stdout, "%s\n", v.str);
    return;
  }
  runtime_error("Unknown value type for emit");
}

static void exec_stmt(Stmt *s) {
  if (!s) return;
  switch (s->kind) {
    case STMT_NOTE: {
      Value v = eval_expr(s->expr);
      store_value_note(s->name, v);
      break;
    }
    case STMT_STAGE: {
      Value v = eval_expr(s->expr);
      store_value_stage(s->name, v);
      break;
    }
    case STMT_EMIT: {
      Value v = eval_expr(s->expr);
      print_value(v);
      break;
    }
    case STMT_BLOCK: {
      push_scope();
      exec_stmt_list(s->list);
      pop_scope();
      break;
    }
    case STMT_BRANCH: {
      Value c = eval_expr(s->cond);
      if (strcmp(c.type, "bool") != 0) runtime_error("branch condition must be bool");
      if (c.boolean) exec_stmt(s->then_block);
      else if (s->else_block) exec_stmt(s->else_block);
      break;
    }
    case STMT_REPEAT: {
      for (;;) {
        Value c = eval_expr(s->cond);
        if (strcmp(c.type, "bool") != 0) runtime_error("repeat condition must be bool");
        if (!c.boolean) break;
        exec_stmt(s->body);
      }
      break;
    }
    default:
      runtime_error("Unknown statement kind");
  }
}

static Stmt *g_main_block = NULL;
%}

%union {
  struct { double num; int is_float; } numlit;
  char *sval;
  struct Expr *expr;
  struct Stmt *stmt;
  struct StmtList *stmt_list;
}

/* keywords */
%token FLOW TAKE EMIT NOTE FIXED STAGE ENSEMBLE PLAY BRANCH ELSEWISE REPEAT SCORE
%token TYPE_INT TYPE_FLOAT TYPE_BOOL TYPE_STRING

/* operators/symbols */
%token EQ ASSIGN AND OR PLUS MINUS MUL DIV LT GT
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE

/* typed tokens */
%token <numlit> NUMBER
%token <sval> STRING_LITERAL
%token <sval> IDENTIFIER

%type <expr> expression
%type <stmt> statement block branch_statement repeat_statement flow_definition
%type <stmt_list> statement_list

%left OR
%left AND
%left EQ
%left LT GT
%left PLUS MINUS
%left MUL DIV

%%

program:
      program flow_definition
    | /* empty */
    ;

flow_definition:
    FLOW IDENTIFIER parameter_part block
    {
      if (!g_main_block) g_main_block = $4;
      free($2);
    }
    ;
parameter_part:
      TAKE LPAREN parameter_list RPAREN
    | /* empty */
    ;

parameter_list:
      parameter_list COMMA IDENTIFIER
        { free($3); }
    | IDENTIFIER
        { free($1); }
    ;

/* ----- block scoping ----- */
block:
  LBRACE statement_list RBRACE { $$ = make_block($2); }
    ;

statement_list:
      statement_list statement { $$ = stmtlist_append($1, $2); }
    | /* empty */ { $$ = NULL; }
    ;

statement:
      NOTE IDENTIFIER ASSIGN expression SEMICOLON
        { $$ = make_assign(STMT_NOTE, $2, $4); }

    | STAGE IDENTIFIER ASSIGN expression SEMICOLON
        { $$ = make_assign(STMT_STAGE, $2, $4); }

    | EMIT expression SEMICOLON
        { $$ = make_emit($2); }

    | branch_statement
    | repeat_statement
    ;

/* ----- branch ----- */
branch_statement:
      BRANCH LPAREN expression RPAREN block
        { $$ = make_branch($3, $5, NULL); }
    | BRANCH LPAREN expression RPAREN block ELSEWISE block
        { $$ = make_branch($3, $5, $7); }
    ;

/* ----- repeat ----- */
repeat_statement:
      REPEAT LPAREN expression RPAREN block
        { $$ = make_repeat($3, $5); }
    ;

/* ----- expressions (type-only) ----- */
expression:
      expression PLUS expression
    { $$ = make_bin(OP_PLUS, $1, $3); }
    | expression MINUS expression
    { $$ = make_bin(OP_MINUS, $1, $3); }
    | expression MUL expression
    { $$ = make_bin(OP_MUL, $1, $3); }
    | expression DIV expression
    { $$ = make_bin(OP_DIV, $1, $3); }

    | expression LT expression
      { $$ = make_bin(OP_LT, $1, $3); }
    | expression GT expression
      { $$ = make_bin(OP_GT, $1, $3); }
    | expression EQ expression
      { $$ = make_bin(OP_EQ, $1, $3); }

    | expression AND expression
      { $$ = make_bin(OP_AND, $1, $3); }
    | expression OR expression
      { $$ = make_bin(OP_OR, $1, $3); }

    | LPAREN expression RPAREN
      { $$ = $2; }

    | NUMBER
      { $$ = make_lit(value_num($1.num, $1.is_float ? "float" : "int")); }

    | STRING_LITERAL
      { $$ = make_lit(value_string($1)); free($1); }

    | IDENTIFIER
      { $$ = make_var($1); }
    ;

%%

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input.txt output.txt\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    if (!input || !output) {
        printf("File error!\n");
        return 1;
    }

    yyin = input;
    yyout = output;

    /* global scope already 0; flow blocks will push/pop */
    yyparse();

    if (g_main_block) {
      exec_stmt(g_main_block);
    }

    fclose(input);
    fclose(output);
    return 0;
}

int yyerror(const char *s) {
    fprintf(yyout ? yyout : stdout, "Syntax Error: %s\n", s);
    return 0;
}