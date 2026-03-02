%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern FILE *yyout;

int yylex();
int yyerror(const char *s);

#include "symbol_table.h"

#include "interpreter.h"

static Stmt *g_main_block = NULL;
%}

%code requires {
  struct Expr;
  struct Stmt;
  struct StmtList;
}

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
%token EQ ASSIGN AND OR PLUS MINUS MUL DIV LT LE GT
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
%left LT LE GT
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
  TAKE LPAREN parameter_list_opt RPAREN
    | /* empty */
    ;

parameter_list_opt:
  parameter_list
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
    | expression LE expression
      { $$ = make_bin(OP_LE, $1, $3); }
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
      { $$ = make_lit_num($1.num, $1.is_float); }

    | STRING_LITERAL
      { $$ = make_lit_string($1); free($1); }

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
      execute_program(g_main_block, yyout);
      free_stmt(g_main_block);
      g_main_block = NULL;
    }

    fclose(input);
    fclose(output);
    return 0;
}

int yyerror(const char *s) {
    fprintf(yyout ? yyout : stdout, "Syntax Error: %s\n", s);
    return 0;
}