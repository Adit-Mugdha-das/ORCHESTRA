
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

#include "interpreter.h"

static Stmt *g_main_block = NULL;

/* When parsing inside a class/ensemble, flow names are qualified as "Class.method". */
static char *g_current_class = NULL;

static char *qualify_name(const char *a, const char *b) {
  char buf[512];
  snprintf(buf, sizeof(buf), "%s.%s", a ? a : "", b ? b : "");
  return arena_strdup(buf);
}


/* Line 189 of yacc.c  */
#line 101 "orchestra.tab.c"

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

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 28 "orchestra.y"

  struct Expr;
  struct Stmt;
  struct StmtList;
  struct ExprList;
  struct NameList;
  struct ChainPart;



/* Line 209 of yacc.c  */
#line 136 "orchestra.tab.c"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FLOW = 258,
     SYMPHONY = 259,
     TAKE = 260,
     EMIT = 261,
     RETURN = 262,
     NOTE = 263,
     FIXED = 264,
     STAGE = 265,
     ENSEMBLE = 266,
     PLAY = 267,
     BRANCH = 268,
     ELSEWISE = 269,
     REPEAT = 270,
     SCORE = 271,
     TYPE_INT = 272,
     TYPE_FLOAT = 273,
     TYPE_BOOL = 274,
     TYPE_STRING = 275,
     TRUE = 276,
     FALSE = 277,
     BREAK = 278,
     CONTINUE = 279,
     EQ = 280,
     NE = 281,
     ASSIGN = 282,
     AND = 283,
     OR = 284,
     PLUS = 285,
     MINUS = 286,
     MUL = 287,
     DIV = 288,
     LT = 289,
     LE = 290,
     GT = 291,
     GE = 292,
     NOT = 293,
     SEMICOLON = 294,
     COMMA = 295,
     DOT = 296,
     LPAREN = 297,
     RPAREN = 298,
     LBRACE = 299,
     RBRACE = 300,
     NUMBER = 301,
     STRING_LITERAL = 302,
     IDENTIFIER = 303,
     UMINUS = 304
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 37 "orchestra.y"

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



/* Line 214 of yacc.c  */
#line 217 "orchestra.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 229 "orchestra.tab.c"

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
#define YYLAST   125

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNRULES -- Number of states.  */
#define YYNSTATES  147

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    13,    14,    21,    24,
      25,    31,    33,    34,    37,    39,    43,    45,    47,    49,
      51,    56,    61,    62,    64,    65,    69,    71,    75,    78,
      79,    85,    91,    99,   103,   107,   110,   116,   124,   127,
     130,   132,   134,   140,   148,   154,   156,   160,   162,   166,
     168,   171,   175,   176,   178,   180,   182,   184,   186,   188,
     192,   196,   198,   202,   206,   208,   211,   214,   216,   220,
     222,   224,   226,   228,   233,   240,   244,   246,   248,   249,
     253
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      51,     0,    -1,    51,    60,    -1,    51,    52,    -1,    51,
      55,    -1,    -1,    -1,     4,    48,    53,    44,    54,    45,
      -1,    54,    60,    -1,    -1,    11,    48,    44,    56,    45,
      -1,    57,    -1,    -1,    57,    58,    -1,    58,    -1,    59,
      48,    39,    -1,    17,    -1,    18,    -1,    19,    -1,    20,
      -1,     3,    48,    61,    64,    -1,     5,    42,    62,    43,
      -1,    -1,    63,    -1,    -1,    63,    40,    48,    -1,    48,
      -1,    44,    65,    45,    -1,    65,    66,    -1,    -1,     8,
      48,    27,    69,    39,    -1,    10,    48,    27,    69,    39,
      -1,    10,    48,    41,    48,    27,    69,    39,    -1,     6,
      69,    39,    -1,     7,    69,    39,    -1,     7,    39,    -1,
      48,    42,    79,    43,    39,    -1,    48,    41,    48,    42,
      79,    43,    39,    -1,    23,    39,    -1,    24,    39,    -1,
      67,    -1,    68,    -1,    13,    42,    69,    43,    64,    -1,
      13,    42,    69,    43,    64,    14,    64,    -1,    15,    42,
      69,    43,    64,    -1,    70,    -1,    70,    29,    71,    -1,
      71,    -1,    71,    28,    72,    -1,    72,    -1,    75,    73,
      -1,    73,    74,    75,    -1,    -1,    34,    -1,    35,    -1,
      36,    -1,    37,    -1,    25,    -1,    26,    -1,    75,    30,
      76,    -1,    75,    31,    76,    -1,    76,    -1,    76,    32,
      77,    -1,    76,    33,    77,    -1,    77,    -1,    31,    77,
      -1,    38,    77,    -1,    78,    -1,    42,    69,    43,    -1,
      21,    -1,    22,    -1,    46,    -1,    47,    -1,    48,    42,
      79,    43,    -1,    48,    41,    48,    42,    79,    43,    -1,
      48,    41,    48,    -1,    48,    -1,    80,    -1,    -1,    80,
      40,    69,    -1,    69,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    89,    89,    90,    91,    92,    98,    97,   110,   111,
     119,   124,   125,   129,   140,   144,   151,   152,   153,   154,
     158,   178,   179,   183,   184,   188,   190,   196,   200,   201,
     205,   208,   211,   214,   217,   220,   223,   226,   229,   231,
     234,   235,   240,   242,   248,   254,   258,   259,   263,   264,
     268,   276,   277,   281,   282,   283,   284,   285,   286,   290,
     291,   292,   296,   297,   298,   302,   303,   304,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   320,   321,   325,
     326
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FLOW", "SYMPHONY", "TAKE", "EMIT",
  "RETURN", "NOTE", "FIXED", "STAGE", "ENSEMBLE", "PLAY", "BRANCH",
  "ELSEWISE", "REPEAT", "SCORE", "TYPE_INT", "TYPE_FLOAT", "TYPE_BOOL",
  "TYPE_STRING", "TRUE", "FALSE", "BREAK", "CONTINUE", "EQ", "NE",
  "ASSIGN", "AND", "OR", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GT",
  "GE", "NOT", "SEMICOLON", "COMMA", "DOT", "LPAREN", "RPAREN", "LBRACE",
  "RBRACE", "NUMBER", "STRING_LITERAL", "IDENTIFIER", "UMINUS", "$accept",
  "program", "symphony_definition", "$@1", "symphony_members",
  "ensemble_definition", "ensemble_fields_opt", "ensemble_fields",
  "ensemble_field", "type_spec", "flow_definition", "parameter_part",
  "parameter_list_opt", "parameter_list", "block", "statement_list",
  "statement", "branch_statement", "repeat_statement", "expression",
  "logic_or", "logic_and", "comparison", "comp_tail", "compop", "additive",
  "term", "unary", "primary", "arg_list_opt", "arg_list", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    51,    51,    51,    53,    52,    54,    54,
      55,    56,    56,    57,    57,    58,    59,    59,    59,    59,
      60,    61,    61,    62,    62,    63,    63,    64,    65,    65,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    67,    67,    68,    69,    70,    70,    71,    71,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    75,
      75,    75,    76,    76,    76,    77,    77,    77,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    79,    79,    80,
      80
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     0,     6,     2,     0,
       5,     1,     0,     2,     1,     3,     1,     1,     1,     1,
       4,     4,     0,     1,     0,     3,     1,     3,     2,     0,
       5,     5,     7,     3,     3,     2,     5,     7,     2,     2,
       1,     1,     5,     7,     5,     1,     3,     1,     3,     1,
       2,     3,     0,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     3,     3,     1,     2,     2,     1,     3,     1,
       1,     1,     1,     4,     6,     3,     1,     1,     0,     3,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     1,     0,     0,     0,     3,     4,     2,    22,
       6,     0,     0,     0,     0,    12,    24,    29,    20,     9,
      16,    17,    18,    19,     0,    11,    14,     0,    26,     0,
      23,     0,     0,    10,    13,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    27,     0,    28,    40,
      41,     7,     8,    15,    25,    69,    70,     0,     0,     0,
      71,    72,    76,     0,    45,    47,    49,    52,    61,    64,
      67,    35,     0,     0,     0,     0,     0,    38,    39,     0,
      78,    65,    66,     0,     0,    78,    33,     0,     0,     0,
       0,    50,     0,     0,    34,     0,     0,     0,     0,     0,
       0,    80,     0,    77,    68,    75,     0,    46,    48,    59,
      60,    57,    58,    53,    54,    55,    56,     0,    62,    63,
       0,     0,     0,     0,     0,    78,     0,     0,    78,    73,
      51,    30,    31,     0,    42,    44,     0,    36,    79,     0,
       0,     0,     0,    74,    32,    43,    37
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     6,    14,    32,     7,    24,    25,    26,    27,
       8,    13,    29,    30,    18,    31,    48,    49,    50,   101,
      64,    65,    66,    91,   117,    67,    68,    69,    70,   102,
     103
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -114
static const yytype_int8 yypact[] =
{
    -114,    30,  -114,   -40,   -35,   -28,  -114,  -114,  -114,    24,
    -114,     4,    17,    19,    32,     7,    -5,  -114,  -114,  -114,
    -114,  -114,  -114,  -114,    16,     7,  -114,    26,  -114,    41,
      35,    -1,     1,  -114,  -114,    47,  -114,    37,    18,    31,
      39,    40,    48,    49,    53,    54,  -114,   -23,  -114,  -114,
    -114,  -114,  -114,  -114,  -114,  -114,  -114,    18,    18,    18,
    -114,  -114,   -10,    55,    67,    69,  -114,     5,    22,  -114,
    -114,  -114,    59,    72,   -24,    18,    18,  -114,  -114,    52,
      18,  -114,  -114,    58,    56,    18,  -114,    18,    18,    18,
      18,    46,    18,    18,  -114,    18,    18,    57,    60,    63,
      65,  -114,    66,    62,  -114,    68,    70,    69,  -114,    22,
      22,  -114,  -114,  -114,  -114,  -114,  -114,    18,  -114,  -114,
      73,    75,    81,    19,    19,    18,    76,    18,    18,  -114,
       5,  -114,  -114,    18,    97,  -114,    74,  -114,  -114,    77,
      79,    19,    80,  -114,  -114,  -114,  -114
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -114,  -114,  -114,  -114,  -114,  -114,  -114,  -114,    91,  -114,
      89,  -114,  -114,  -114,  -113,  -114,  -114,  -114,  -114,   -38,
    -114,    36,    34,  -114,  -114,     8,   -22,   -42,  -114,   -83,
    -114
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      63,    72,   106,    96,     3,    38,    39,    40,     9,    41,
     134,   135,    42,    10,    43,    81,    82,    97,    79,    80,
      11,    83,    44,    45,    20,    21,    22,    23,   145,    12,
       2,    84,    85,     3,     4,    89,    90,    98,    99,    55,
      56,     5,   136,    28,    46,   139,    51,    47,    15,    57,
     118,   119,    55,    56,    92,    93,    58,   120,   121,    16,
      59,    33,    57,    17,    60,    61,    62,   109,   110,    58,
      71,   111,   112,    59,    35,    37,    19,    60,    61,    62,
     113,   114,   115,   116,    36,    54,    53,    73,    74,   138,
      75,    76,    77,    78,    86,   140,    87,    88,    94,    95,
     100,   104,   127,   123,   105,   122,   124,   125,   133,   126,
     128,   141,   131,   129,   132,   137,    34,   142,   144,   146,
     143,    52,   108,   107,     0,   130
};

static const yytype_int16 yycheck[] =
{
      38,    39,    85,    27,     3,     6,     7,     8,    48,    10,
     123,   124,    13,    48,    15,    57,    58,    41,    41,    42,
      48,    59,    23,    24,    17,    18,    19,    20,   141,     5,
       0,    41,    42,     3,     4,    30,    31,    75,    76,    21,
      22,    11,   125,    48,    45,   128,    45,    48,    44,    31,
      92,    93,    21,    22,    32,    33,    38,    95,    96,    42,
      42,    45,    31,    44,    46,    47,    48,    89,    90,    38,
      39,    25,    26,    42,    48,    40,    44,    46,    47,    48,
      34,    35,    36,    37,    43,    48,    39,    48,    48,   127,
      42,    42,    39,    39,    39,   133,    29,    28,    39,    27,
      48,    43,    40,    43,    48,    48,    43,    42,    27,    43,
      42,    14,    39,    43,    39,    39,    25,    43,    39,    39,
      43,    32,    88,    87,    -1,   117
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    51,     0,     3,     4,    11,    52,    55,    60,    48,
      48,    48,     5,    61,    53,    44,    42,    44,    64,    44,
      17,    18,    19,    20,    56,    57,    58,    59,    48,    62,
      63,    65,    54,    45,    58,    48,    43,    40,     6,     7,
       8,    10,    13,    15,    23,    24,    45,    48,    66,    67,
      68,    45,    60,    39,    48,    21,    22,    31,    38,    42,
      46,    47,    48,    69,    70,    71,    72,    75,    76,    77,
      78,    39,    69,    48,    48,    42,    42,    39,    39,    41,
      42,    77,    77,    69,    41,    42,    39,    29,    28,    30,
      31,    73,    32,    33,    39,    27,    27,    41,    69,    69,
      48,    69,    79,    80,    43,    48,    79,    71,    72,    76,
      76,    25,    26,    34,    35,    36,    37,    74,    77,    77,
      69,    69,    48,    43,    43,    42,    43,    40,    42,    43,
      75,    39,    39,    27,    64,    64,    79,    39,    69,    79,
      69,    14,    43,    43,    39,    64,    39
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
        case 6:

/* Line 1455 of yacc.c  */
#line 98 "orchestra.y"
    {
      /* Enter class/ensemble scope for name qualification. */
      g_current_class = (yyvsp[(2) - (2)].sval);
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 103 "orchestra.y"
    {
      /* Exit class/ensemble scope. */
      g_current_class = NULL;
    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 120 "orchestra.y"
    { register_ensemble((yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].field_list)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 125 "orchestra.y"
    { (yyval.field_list) = NULL; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 130 "orchestra.y"
    {
        /* append single-node list */
        FieldList *tail = (yyvsp[(1) - (2)].field_list);
        if (!tail) (yyval.field_list) = (yyvsp[(2) - (2)].field_list);
        else {
          while (tail->next) tail = tail->next;
          tail->next = (yyvsp[(2) - (2)].field_list);
          (yyval.field_list) = (yyvsp[(1) - (2)].field_list);
        }
      ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 140 "orchestra.y"
    { (yyval.field_list) = (yyvsp[(1) - (1)].field_list); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 145 "orchestra.y"
    {
        (yyval.field_list) = fieldlist_append(NULL, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval));
      ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 151 "orchestra.y"
    { (yyval.sval) = arena_strdup("int"); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 152 "orchestra.y"
    { (yyval.sval) = arena_strdup("float"); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 153 "orchestra.y"
    { (yyval.sval) = arena_strdup("bool"); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 154 "orchestra.y"
    { (yyval.sval) = arena_strdup("string"); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 159 "orchestra.y"
    {
      char *fname = (yyvsp[(2) - (4)].sval);
      if (g_current_class) {
        fname = qualify_name(g_current_class, (yyvsp[(2) - (4)].sval));
      }

      register_flow(fname, (yyvsp[(3) - (4)].name_list), (yyvsp[(4) - (4)].stmt));

      /* Only top-level flows participate in entry selection. */
      if (!g_current_class) {
        if (strcmp((yyvsp[(2) - (4)].sval), "main") == 0) {
          g_main_block = (yyvsp[(4) - (4)].stmt);
        } else if (!g_main_block) {
          g_main_block = (yyvsp[(4) - (4)].stmt);
        }
      }
    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 178 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(3) - (4)].name_list); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 179 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 183 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(1) - (1)].name_list); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 184 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 189 "orchestra.y"
    { (yyval.name_list) = namelist_append((yyvsp[(1) - (3)].name_list), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 191 "orchestra.y"
    { (yyval.name_list) = namelist_append(NULL, (yyvsp[(1) - (1)].sval)); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 196 "orchestra.y"
    { (yyval.stmt) = make_block((yyvsp[(2) - (3)].stmt_list)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 200 "orchestra.y"
    { (yyval.stmt_list) = stmtlist_append((yyvsp[(1) - (2)].stmt_list), (yyvsp[(2) - (2)].stmt)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 201 "orchestra.y"
    { (yyval.stmt_list) = NULL; ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 206 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_NOTE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 209 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_STAGE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 212 "orchestra.y"
    { (yyval.stmt) = make_field_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].sval), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 215 "orchestra.y"
    { (yyval.stmt) = make_emit((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 218 "orchestra.y"
    { (yyval.stmt) = make_return((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 221 "orchestra.y"
    { (yyval.stmt) = make_return(NULL); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 224 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_call((yyvsp[(1) - (5)].sval), (yyvsp[(3) - (5)].expr_list))); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 227 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_call(qualify_name((yyvsp[(1) - (7)].sval), (yyvsp[(3) - (7)].sval)), (yyvsp[(5) - (7)].expr_list))); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 230 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_BREAK, NULL, NULL); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 232 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_CONTINUE, NULL, NULL); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 241 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt), NULL); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 243 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 249 "orchestra.y"
    { (yyval.stmt) = make_repeat((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt)); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 254 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 258 "orchestra.y"
    { (yyval.expr) = make_bin(OP_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 259 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 263 "orchestra.y"
    { (yyval.expr) = make_bin(OP_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 264 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 269 "orchestra.y"
    {
        if ((yyvsp[(2) - (2)].chain_parts)) (yyval.expr) = make_chain((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].chain_parts));
        else (yyval.expr) = (yyvsp[(1) - (2)].expr);
      ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 276 "orchestra.y"
    { (yyval.chain_parts) = chainpart_append((yyvsp[(1) - (3)].chain_parts), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 277 "orchestra.y"
    { (yyval.chain_parts) = NULL; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 281 "orchestra.y"
    { (yyval.ival) = OP_LT; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 282 "orchestra.y"
    { (yyval.ival) = OP_LE; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 283 "orchestra.y"
    { (yyval.ival) = OP_GT; ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 284 "orchestra.y"
    { (yyval.ival) = OP_GE; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 285 "orchestra.y"
    { (yyval.ival) = OP_EQ; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 286 "orchestra.y"
    { (yyval.ival) = OP_NE; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 290 "orchestra.y"
    { (yyval.expr) = make_bin(OP_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 291 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 292 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 296 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MUL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 297 "orchestra.y"
    { (yyval.expr) = make_bin(OP_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 298 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 302 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NEG, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 303 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NOT, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 304 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 308 "orchestra.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 309 "orchestra.y"
    { (yyval.expr) = make_lit_bool(1); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 310 "orchestra.y"
    { (yyval.expr) = make_lit_bool(0); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 311 "orchestra.y"
    { (yyval.expr) = make_lit_num((yyvsp[(1) - (1)].numlit).num, (yyvsp[(1) - (1)].numlit).is_float); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 312 "orchestra.y"
    { (yyval.expr) = make_lit_string((yyvsp[(1) - (1)].sval)); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 313 "orchestra.y"
    { (yyval.expr) = make_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 314 "orchestra.y"
    { (yyval.expr) = make_call(qualify_name((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].sval)), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 315 "orchestra.y"
    { (yyval.expr) = make_field((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 316 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 320 "orchestra.y"
    { (yyval.expr_list) = (yyvsp[(1) - (1)].expr_list); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 321 "orchestra.y"
    { (yyval.expr_list) = NULL; ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 325 "orchestra.y"
    { (yyval.expr_list) = exprlist_append((yyvsp[(1) - (3)].expr_list), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 326 "orchestra.y"
    { (yyval.expr_list) = exprlist_append(NULL, (yyvsp[(1) - (1)].expr)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2082 "orchestra.tab.c"
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
#line 329 "orchestra.y"


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
