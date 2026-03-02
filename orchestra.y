%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern FILE *yyout;

int yylex();
int yyerror(const char *s);
%}

/* ---------- Token Definitions ---------- */

%union {
    float fval;
    char* sval;
}

%token FLOW TAKE EMIT NOTE FIXED STAGE ENSEMBLE PLAY BRANCH ELSEWISE REPEAT SCORE
%token TYPE_INT TYPE_FLOAT TYPE_BOOL TYPE_STRING

%token EQ ASSIGN AND OR PLUS MINUS MUL DIV LT GT
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE

%token <fval> NUMBER
%token <sval> IDENTIFIER STRING_LITERAL

%type <fval> expression

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
      FLOW IDENTIFIER LBRACE statement_list RBRACE
        { fprintf(yyout, "Parsed flow: %s\n", $2); }
    ;

statement_list:
      statement_list statement
    | /* empty */
    ;

statement:
      NOTE IDENTIFIER ASSIGN expression SEMICOLON
        { fprintf(yyout, "Variable declared: %s\n", $2); }
    | STAGE IDENTIFIER ASSIGN expression SEMICOLON
        { fprintf(yyout, "Stage assignment: %s\n", $2); }
    | EMIT expression SEMICOLON
        { fprintf(yyout, "Emit statement\n"); }
    ;

expression:
      expression PLUS expression   { $$ = $1 + $3; }
    | expression MINUS expression  { $$ = $1 - $3; }
    | expression MUL expression    { $$ = $1 * $3; }
    | expression DIV expression    { $$ = $1 / $3; }
    | LPAREN expression RPAREN     { $$ = $2; }
    | NUMBER                       { $$ = $1; }
    | IDENTIFIER                   { $$ = 0; }   /* allow variables in expressions */
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

    yyparse();

    fclose(input);
    fclose(output);

    return 0;
}

int yyerror(const char *s) {
    fprintf(yyout, "Syntax Error: %s\n", s);
    return 0;
}