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

/* When parsing inside a class/ensemble, flow names are qualified as "Class.method". */
static char *g_current_class = NULL;

static char *qualify_name(const char *a, const char *b) {
  char buf[512];
  snprintf(buf, sizeof(buf), "%s.%s", a ? a : "", b ? b : "");
  return arena_strdup(buf);
}
%}

%code requires {
  struct Expr;
  struct Stmt;
  struct StmtList;
  struct ExprList;
  struct NameList;
  struct ChainPart;
}

%union {
  struct { double num; int is_float; } numlit;
  char *sval;
  int ival;
  struct Expr *expr;
  struct Stmt *stmt;
  struct StmtList *stmt_list;
  struct ExprList *expr_list;
  struct NameList *name_list;
  struct ChainPart *chain_parts;
  struct FieldList *field_list;
}

/* keywords */
%token FLOW SYMPHONY TAKE EMIT RETURN NOTE FIXED STAGE ENSEMBLE PLAY BRANCH ELSEWISE REPEAT SCORE
%token TYPE_INT TYPE_FLOAT TYPE_BOOL TYPE_STRING
%token TRUE FALSE
%token BREAK CONTINUE

/* operators/symbols */
%token EQ NE ASSIGN AND OR PLUS MINUS MUL DIV LT LE GT GE NOT
%token SEMICOLON COMMA DOT LPAREN RPAREN LBRACE RBRACE

/* typed tokens */
%token <numlit> NUMBER
%token <sval> STRING_LITERAL
%token <sval> IDENTIFIER

%type <expr> expression logic_or logic_and comparison additive term unary primary
%type <stmt> statement block branch_statement repeat_statement flow_definition
%type <stmt_list> statement_list
%type <expr_list> arg_list arg_list_opt
%type <name_list> parameter_part parameter_list_opt parameter_list
%type <chain_parts> comp_tail
%type <ival> compop


%type <sval> type_spec
%type <field_list> ensemble_fields_opt ensemble_fields ensemble_field

%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left MUL DIV
%right NOT
%nonassoc UMINUS

%%

program:
      program flow_definition
    | program symphony_definition
    | program ensemble_definition
    | /* empty */
    ;

/* ---- symphony (class-like namespace MVP) ---- */
symphony_definition:
    SYMPHONY IDENTIFIER
    {
      /* Enter class/ensemble scope for name qualification. */
      g_current_class = $2;
    }
    LBRACE symphony_members RBRACE
    {
      /* Exit class/ensemble scope. */
      g_current_class = NULL;
    }
    ;

symphony_members:
      symphony_members flow_definition
    | /* empty */
    ;

/* ---- ensemble (struct-like declaration MVP) ----
   For now this only parses and is ignored at runtime.
   Next step can store layout + allow instances/field access.
*/
ensemble_definition:
    ENSEMBLE IDENTIFIER LBRACE ensemble_fields_opt RBRACE
    { register_ensemble($2, $4); }
    ;

ensemble_fields_opt:
      ensemble_fields
    | /* empty */ { $$ = NULL; }
    ;

ensemble_fields:
      ensemble_fields ensemble_field
      {
        /* append single-node list */
        FieldList *tail = $1;
        if (!tail) $$ = $2;
        else {
          while (tail->next) tail = tail->next;
          tail->next = $2;
          $$ = $1;
        }
      }
    | ensemble_field { $$ = $1; }
    ;

ensemble_field:
      type_spec IDENTIFIER SEMICOLON
      {
        $$ = fieldlist_append(NULL, $1, $2);
      }
    ;

type_spec:
      TYPE_INT { $$ = arena_strdup("int"); }
    | TYPE_FLOAT { $$ = arena_strdup("float"); }
    | TYPE_BOOL { $$ = arena_strdup("bool"); }
    | TYPE_STRING { $$ = arena_strdup("string"); }
    ;

flow_definition:
    FLOW IDENTIFIER parameter_part block
    {
      char *fname = $2;
      if (g_current_class) {
        fname = qualify_name(g_current_class, $2);
      }

      register_flow(fname, $3, $4);

      /* Only top-level flows participate in entry selection. */
      if (!g_current_class) {
        if (strcmp($2, "main") == 0) {
          g_main_block = $4;
        } else if (!g_main_block) {
          g_main_block = $4;
        }
      }
    }
    ;
parameter_part:
  TAKE LPAREN parameter_list_opt RPAREN { $$ = $3; }
    | /* empty */ { $$ = NULL; }
    ;

parameter_list_opt:
  parameter_list { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

parameter_list:
      parameter_list COMMA IDENTIFIER
    { $$ = namelist_append($1, $3); }
    | IDENTIFIER
    { $$ = namelist_append(NULL, $1); }
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

    | STAGE IDENTIFIER DOT IDENTIFIER ASSIGN expression SEMICOLON
      { $$ = make_field_stage($2, $4, $6); }

    | EMIT expression SEMICOLON
        { $$ = make_emit($2); }

    | RETURN expression SEMICOLON
      { $$ = make_return($2); }

    | RETURN SEMICOLON
      { $$ = make_return(NULL); }

    | IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON
      { $$ = make_expr_stmt(make_call($1, $3)); }

    | IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN SEMICOLON
      { $$ = make_expr_stmt(make_call(qualify_name($1, $3), $5)); }

    | BREAK SEMICOLON
      { $$ = make_assign(STMT_BREAK, NULL, NULL); }
    | CONTINUE SEMICOLON
      { $$ = make_assign(STMT_CONTINUE, NULL, NULL); }

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

/* ----- expressions ----- */
expression:
      logic_or { $$ = $1; }
    ;

logic_or:
      logic_or OR logic_and { $$ = make_bin(OP_OR, $1, $3); }
    | logic_and { $$ = $1; }
    ;

logic_and:
  logic_and AND comparison { $$ = make_bin(OP_AND, $1, $3); }
    | comparison { $$ = $1; }
    ;

comparison:
  additive comp_tail
      {
        if ($2) $$ = make_chain($1, $2);
        else $$ = $1;
      }
    ;

comp_tail:
  comp_tail compop additive { $$ = chainpart_append($1, $2, $3); }
    | /* empty */ { $$ = NULL; }
    ;

compop:
      LT { $$ = OP_LT; }
    | LE { $$ = OP_LE; }
    | GT { $$ = OP_GT; }
    | GE { $$ = OP_GE; }
    | EQ { $$ = OP_EQ; }
    | NE { $$ = OP_NE; }
    ;

additive:
      additive PLUS term { $$ = make_bin(OP_PLUS, $1, $3); }
    | additive MINUS term { $$ = make_bin(OP_MINUS, $1, $3); }
    | term { $$ = $1; }
    ;

term:
      term MUL unary { $$ = make_bin(OP_MUL, $1, $3); }
    | term DIV unary { $$ = make_bin(OP_DIV, $1, $3); }
    | unary { $$ = $1; }
    ;

unary:
      MINUS unary %prec UMINUS { $$ = make_unary(OP_NEG, $2); }
    | NOT unary { $$ = make_unary(OP_NOT, $2); }
    | primary { $$ = $1; }
    ;

primary:
      LPAREN expression RPAREN { $$ = $2; }
    | TRUE { $$ = make_lit_bool(1); }
    | FALSE { $$ = make_lit_bool(0); }
    | NUMBER { $$ = make_lit_num($1.num, $1.is_float); }
    | STRING_LITERAL { $$ = make_lit_string($1); free($1); }
    | IDENTIFIER LPAREN arg_list_opt RPAREN { $$ = make_call($1, $3); }
    | IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN { $$ = make_call(qualify_name($1, $3), $5); }
    | IDENTIFIER DOT IDENTIFIER { $$ = make_field($1, $3); }
    | IDENTIFIER { $$ = make_var($1); }
    ;

arg_list_opt:
      arg_list { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

arg_list:
      arg_list COMMA expression { $$ = exprlist_append($1, $3); }
    | expression { $$ = exprlist_append(NULL, $1); }
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
      g_main_block = NULL;
    }

    free_all_flows();
    arena_free_all();

    fclose(input);
    fclose(output);
    return 0;
}

int yyerror(const char *s) {
    fprintf(yyout ? yyout : stdout, "Syntax Error: %s\n", s);
    return 0;
}