
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "orchestra.y"

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


/* Line 189 of yacc.c  */
#line 416 "orchestra.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FLOW = 258,
     TAKE = 259,
     EMIT = 260,
     NOTE = 261,
     FIXED = 262,
     STAGE = 263,
     ENSEMBLE = 264,
     PLAY = 265,
     BRANCH = 266,
     ELSEWISE = 267,
     REPEAT = 268,
     SCORE = 269,
     TYPE_INT = 270,
     TYPE_FLOAT = 271,
     TYPE_BOOL = 272,
     TYPE_STRING = 273,
     EQ = 274,
     ASSIGN = 275,
     AND = 276,
     OR = 277,
     PLUS = 278,
     MINUS = 279,
     MUL = 280,
     DIV = 281,
     LT = 282,
     GT = 283,
     SEMICOLON = 284,
     COMMA = 285,
     LPAREN = 286,
     RPAREN = 287,
     LBRACE = 288,
     RBRACE = 289,
     NUMBER = 290,
     STRING_LITERAL = 291,
     IDENTIFIER = 292
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 343 "orchestra.y"

  struct { double num; int is_float; } numlit;
  char *sval;
  struct Expr *expr;
  struct Stmt *stmt;
  struct StmtList *stmt_list;



/* Line 214 of yacc.c  */
#line 499 "orchestra.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 511 "orchestra.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   130

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  32
/* YYNRULES -- Number of states.  */
#define YYNSTATES  70

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     7,    12,    17,    18,    22,    24,
      28,    31,    32,    38,    44,    48,    50,    52,    58,    66,
      72,    76,    80,    84,    88,    92,    96,   100,   104,   108,
     112,   114,   116
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      39,     0,    -1,    39,    40,    -1,    -1,     3,    37,    41,
      43,    -1,     4,    31,    42,    32,    -1,    -1,    42,    30,
      37,    -1,    37,    -1,    33,    44,    34,    -1,    44,    45,
      -1,    -1,     6,    37,    20,    48,    29,    -1,     8,    37,
      20,    48,    29,    -1,     5,    48,    29,    -1,    46,    -1,
      47,    -1,    11,    31,    48,    32,    43,    -1,    11,    31,
      48,    32,    43,    12,    43,    -1,    13,    31,    48,    32,
      43,    -1,    48,    23,    48,    -1,    48,    24,    48,    -1,
      48,    25,    48,    -1,    48,    26,    48,    -1,    48,    27,
      48,    -1,    48,    28,    48,    -1,    48,    19,    48,    -1,
      48,    21,    48,    -1,    48,    22,    48,    -1,    31,    48,
      32,    -1,    35,    -1,    36,    -1,    37,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   378,   378,   379,   383,   390,   391,   395,   397,   403,
     407,   408,   412,   415,   418,   421,   422,   427,   429,   435,
     441,   443,   445,   447,   450,   452,   454,   457,   459,   462,
     465,   468,   471
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FLOW", "TAKE", "EMIT", "NOTE", "FIXED",
  "STAGE", "ENSEMBLE", "PLAY", "BRANCH", "ELSEWISE", "REPEAT", "SCORE",
  "TYPE_INT", "TYPE_FLOAT", "TYPE_BOOL", "TYPE_STRING", "EQ", "ASSIGN",
  "AND", "OR", "PLUS", "MINUS", "MUL", "DIV", "LT", "GT", "SEMICOLON",
  "COMMA", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "NUMBER",
  "STRING_LITERAL", "IDENTIFIER", "$accept", "program", "flow_definition",
  "parameter_part", "parameter_list", "block", "statement_list",
  "statement", "branch_statement", "repeat_statement", "expression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    39,    40,    41,    41,    42,    42,    43,
      44,    44,    45,    45,    45,    45,    45,    46,    46,    47,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     4,     4,     0,     3,     1,     3,
       2,     0,     5,     5,     3,     1,     1,     5,     7,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,     2,     6,     0,     0,     0,    11,
       4,     8,     0,     0,     0,     5,     0,     0,     0,     0,
       0,     9,    10,    15,    16,     7,     0,    30,    31,    32,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
      29,    26,    27,    28,    20,    21,    22,    23,    24,    25,
       0,     0,     0,     0,    12,    13,    17,    19,     0,    18
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     4,     7,    12,    10,    13,    22,    23,    24,
      30
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -33
static const yytype_int8 yypact[] =
{
     -33,     4,   -33,   -32,   -33,     7,    -7,    17,    14,   -33,
     -33,   -33,    68,    -5,    25,   -33,   -10,    26,    39,    77,
      79,   -33,   -33,   -33,   -33,   -33,   -10,   -33,   -33,   -33,
      45,    89,   110,   -10,   -10,     9,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -33,   -10,   -10,    21,    33,
     -33,   101,    88,    78,    13,    13,   -33,   -33,    96,    96,
      56,    67,    17,    17,   -33,   -33,    75,   -33,    17,   -33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -33,   -33,   -33,   -33,   -33,    55,   -33,   -33,   -33,   -33,
     -24
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      16,    17,    35,    18,     2,     5,    19,     3,    20,    48,
      49,     6,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    26,    60,    61,     8,    27,    28,    29,    36,    21,
      37,    38,    39,    40,    41,    42,    43,    44,    41,    42,
      36,    50,    37,    38,    39,    40,    41,    42,    43,    44,
       9,    11,    36,    62,    37,    38,    39,    40,    41,    42,
      43,    44,    25,    31,    36,    63,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    36,    32,    37,    38,    39,
      40,    41,    42,    43,    44,    64,    36,    68,    37,    38,
      39,    40,    41,    42,    43,    44,    65,    36,    14,    37,
      15,    39,    40,    41,    42,    43,    44,    36,    33,    46,
      34,    39,    40,    41,    42,    43,    44,    66,    67,    39,
      40,    41,    42,    69,    39,    40,    41,    42,    43,    44,
      47
};

static const yytype_uint8 yycheck[] =
{
       5,     6,    26,     8,     0,    37,    11,     3,    13,    33,
      34,     4,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    31,    46,    47,    31,    35,    36,    37,    19,    34,
      21,    22,    23,    24,    25,    26,    27,    28,    25,    26,
      19,    32,    21,    22,    23,    24,    25,    26,    27,    28,
      33,    37,    19,    32,    21,    22,    23,    24,    25,    26,
      27,    28,    37,    37,    19,    32,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    19,    37,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    19,    12,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    19,    30,    21,
      32,    23,    24,    25,    26,    27,    28,    19,    31,    20,
      31,    23,    24,    25,    26,    27,    28,    62,    63,    23,
      24,    25,    26,    68,    23,    24,    25,    26,    27,    28,
      20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    39,     0,     3,    40,    37,     4,    41,    31,    33,
      43,    37,    42,    44,    30,    32,     5,     6,     8,    11,
      13,    34,    45,    46,    47,    37,    31,    35,    36,    37,
      48,    37,    37,    31,    31,    48,    19,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    20,    20,    48,    48,
      32,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    32,    32,    29,    29,    43,    43,    12,    43
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1455 of yacc.c  */
#line 384 "orchestra.y"
    {
      if (!g_main_block) g_main_block = (yyvsp[(4) - (4)].stmt);
      free((yyvsp[(2) - (4)].sval));
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 396 "orchestra.y"
    { free((yyvsp[(3) - (3)].sval)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 398 "orchestra.y"
    { free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 403 "orchestra.y"
    { (yyval.stmt) = make_block((yyvsp[(2) - (3)].stmt_list)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 407 "orchestra.y"
    { (yyval.stmt_list) = stmtlist_append((yyvsp[(1) - (2)].stmt_list), (yyvsp[(2) - (2)].stmt)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 408 "orchestra.y"
    { (yyval.stmt_list) = NULL; ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 413 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_NOTE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 416 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_STAGE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 419 "orchestra.y"
    { (yyval.stmt) = make_emit((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 428 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt), NULL); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 430 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt)); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 436 "orchestra.y"
    { (yyval.stmt) = make_repeat((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt)); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 442 "orchestra.y"
    { (yyval.expr) = make_bin(OP_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 444 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 446 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MUL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 448 "orchestra.y"
    { (yyval.expr) = make_bin(OP_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 451 "orchestra.y"
    { (yyval.expr) = make_bin(OP_LT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 453 "orchestra.y"
    { (yyval.expr) = make_bin(OP_GT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 455 "orchestra.y"
    { (yyval.expr) = make_bin(OP_EQ, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 458 "orchestra.y"
    { (yyval.expr) = make_bin(OP_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 460 "orchestra.y"
    { (yyval.expr) = make_bin(OP_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 463 "orchestra.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 466 "orchestra.y"
    { (yyval.expr) = make_lit(value_num((yyvsp[(1) - (1)].numlit).num, (yyvsp[(1) - (1)].numlit).is_float ? "float" : "int")); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 469 "orchestra.y"
    { (yyval.expr) = make_lit(value_string((yyvsp[(1) - (1)].sval))); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 472 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 1948 "orchestra.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 475 "orchestra.y"


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
