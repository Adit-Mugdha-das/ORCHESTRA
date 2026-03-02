%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern FILE *yyout;

int yylex();
int yyerror(const char *s);

#include "symbol_table.h"

/* ----- helpers ----- */
static void type_error(const char *msg) {
    fprintf(yyout ? yyout : stdout, "Type Error: %s\n", msg);
    exit(1);
}

static int is_numeric(const char *t) {
    return (strcmp(t, "int") == 0 || strcmp(t, "float") == 0);
}

static int is_bool(const char *t) {
    return (strcmp(t, "bool") == 0);
}

/* unknown inference: if one side is unknown, it becomes the other side */
static const char* unify_unknown(const char *a, const char *b) {
    if (strcmp(a, "unknown") == 0) return b;
    if (strcmp(b, "unknown") == 0) return a;
    return NULL; // no inference needed
}

static void set_type(char dst[16], const char *src) {
    strncpy(dst, src, 15);
    dst[15] = '\0';
}

/* numeric promotion:
   - int op int -> int (except DIV -> float; we choose Python-like: / gives float)
   - float involved -> float
*/
static void promote_numeric(char out[16], const char *a, const char *b, int is_div) {
    if (!is_numeric(a) || !is_numeric(b)) type_error("Arithmetic requires numeric types");
    if (is_div) { set_type(out, "float"); return; }
    if (strcmp(a, "float") == 0 || strcmp(b, "float") == 0) set_type(out, "float");
    else set_type(out, "int");
}
%}

%union {
    struct { char type[16]; } expr;
    char* sval;
}

/* keywords */
%token FLOW TAKE EMIT NOTE FIXED STAGE ENSEMBLE PLAY BRANCH ELSEWISE REPEAT SCORE
%token TYPE_INT TYPE_FLOAT TYPE_BOOL TYPE_STRING

/* operators/symbols */
%token EQ ASSIGN AND OR PLUS MINUS MUL DIV LT GT
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE

/* typed tokens */
%token <expr> NUMBER
%token <expr> STRING_LITERAL
%token <sval> IDENTIFIER

%type <expr> expression

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
      FLOW IDENTIFIER
      {
          push_scope();   // Flow scope begins here
      }
      parameter_part
      block
      {
          pop_scope();    // End flow scope
          fprintf(yyout, "Parsed flow: %s\n", $2);
          free($2);
      }
    ;
parameter_part:
      TAKE LPAREN parameter_list RPAREN
    | /* empty */
    ;

parameter_list:
      parameter_list COMMA IDENTIFIER
        { /* infer later: start as unknown */
          declare_or_update_current_scope($3, "unknown");
          free($3);
        }
    | IDENTIFIER
        { declare_or_update_current_scope($1, "unknown"); free($1); }
    ;

/* ----- block scoping ----- */
block:
      LBRACE { push_scope(); } statement_list RBRACE { pop_scope(); }
    ;

statement_list:
      statement_list statement
    | /* empty */
    ;

statement:
      NOTE IDENTIFIER ASSIGN expression SEMICOLON
        {
          /* "note" behaves like python binding but scoped: creates/updates in current scope */
          declare_or_update_current_scope($2, $4.type);
          fprintf(yyout, "note %s : %s\n", $2, $4.type);
          free($2);
        }

    | STAGE IDENTIFIER ASSIGN expression SEMICOLON
        {
          /* "stage" updates nearest binding; if not exists, create in current scope (python-like) */
          insert_or_update($2, $4.type);
          fprintf(yyout, "stage %s : %s\n", $2, $4.type);
          free($2);
        }

    | EMIT expression SEMICOLON
        { fprintf(yyout, "emit (%s)\n", $2.type); }

    | branch_statement
    | repeat_statement
    ;

/* ----- branch ----- */
branch_statement:
      BRANCH LPAREN expression RPAREN block
        {
          if (!is_bool($3.type)) type_error("branch condition must be bool");
          fprintf(yyout, "branch (no else)\n");
        }
    | BRANCH LPAREN expression RPAREN block ELSEWISE block
        {
          if (!is_bool($3.type)) type_error("branch condition must be bool");
          fprintf(yyout, "branch (with else)\n");
        }
    ;

/* ----- repeat ----- */
repeat_statement:
      REPEAT LPAREN expression RPAREN block
        {
          if (!is_bool($3.type)) type_error("repeat condition must be bool");
          fprintf(yyout, "repeat loop\n");
        }
    ;

/* ----- expressions (type-only) ----- */
expression:
      expression PLUS expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) { set_type($$.type, inf); }
          else {
            promote_numeric($$.type, $1.type, $3.type, 0);
          }
        }
    | expression MINUS expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) { set_type($$.type, inf); }
          else {
            promote_numeric($$.type, $1.type, $3.type, 0);
          }
        }
    | expression MUL expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) { set_type($$.type, inf); }
          else {
            promote_numeric($$.type, $1.type, $3.type, 0);
          }
        }
    | expression DIV expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) { set_type($$.type, "float"); }
          else {
            promote_numeric($$.type, $1.type, $3.type, 1);
          }
        }

    | expression LT expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) {
            if (!is_numeric(inf)) type_error("LT comparison supports numeric types");
          } else {
            if (!is_numeric($1.type) || !is_numeric($3.type)) type_error("LT comparison supports numeric types");
          }
          set_type($$.type, "bool");
        }
    | expression GT expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) {
            if (!is_numeric(inf)) type_error("GT comparison supports numeric types");
          } else {
            if (!is_numeric($1.type) || !is_numeric($3.type)) type_error("GT comparison supports numeric types");
          }
          set_type($$.type, "bool");
        }
    | expression EQ expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (!inf && strcmp($1.type, $3.type) != 0) {
            if (!(is_numeric($1.type) && is_numeric($3.type))) type_error("EQ operands must have same type");
          }
          set_type($$.type, "bool");
        }

    | expression AND expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) { if (strcmp(inf, "bool") != 0) type_error("&& requires bool"); }
          else {
            if (!is_bool($1.type) || !is_bool($3.type)) type_error("&& requires bool");
          }
          set_type($$.type, "bool");
        }
    | expression OR expression
        {
          const char* inf = unify_unknown($1.type, $3.type);
          if (inf) { if (strcmp(inf, "bool") != 0) type_error("|| requires bool"); }
          else {
            if (!is_bool($1.type) || !is_bool($3.type)) type_error("|| requires bool");
          }
          set_type($$.type, "bool");
        }

    | LPAREN expression RPAREN
        { set_type($$.type, $2.type); }

    | NUMBER
        { set_type($$.type, $1.type); }

    | STRING_LITERAL
        { set_type($$.type, $1.type); }

    | IDENTIFIER
        {
          const char *t = get_type_or_error($1);
          set_type($$.type, t);
          free($1);
        }
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

    fclose(input);
    fclose(output);
    return 0;
}

int yyerror(const char *s) {
    fprintf(yyout ? yyout : stdout, "Syntax Error: %s\n", s);
    return 0;
}