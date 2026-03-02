
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
static FieldList *g_current_symphony_fields = NULL;

static char *qualify_name(const char *a, const char *b) {
  char buf[512];
  snprintf(buf, sizeof(buf), "%s.%s", a ? a : "", b ? b : "");
  return arena_strdup(buf);
}


/* Line 189 of yacc.c  */
#line 102 "orchestra.tab.c"

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
#line 29 "orchestra.y"

  struct Expr;
  struct Stmt;
  struct StmtList;
  struct ExprList;
  struct NameList;
  struct ChainPart;



/* Line 209 of yacc.c  */
#line 137 "orchestra.tab.c"

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
     THIS = 304,
     UMINUS = 305
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 38 "orchestra.y"

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
#line 219 "orchestra.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 231 "orchestra.tab.c"

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
#define YYLAST   147

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  87
/* YYNRULES -- Number of states.  */
#define YYNSTATES  166

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    13,    14,    21,    24,
      25,    27,    31,    37,    39,    40,    43,    45,    49,    51,
      53,    55,    57,    62,    67,    68,    70,    71,    75,    77,
      81,    84,    85,    91,    97,   105,   113,   117,   121,   124,
     130,   138,   141,   144,   146,   148,   154,   162,   168,   170,
     174,   176,   180,   182,   185,   189,   190,   192,   194,   196,
     198,   200,   202,   206,   210,   212,   216,   220,   222,   225,
     228,   230,   234,   236,   238,   240,   242,   247,   254,   258,
     263,   270,   274,   276,   278,   280,   281,   285
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      52,     0,    -1,    52,    62,    -1,    52,    53,    -1,    52,
      57,    -1,    -1,    -1,     4,    48,    54,    44,    55,    45,
      -1,    55,    56,    -1,    -1,    62,    -1,    61,    48,    39,
      -1,    11,    48,    44,    58,    45,    -1,    59,    -1,    -1,
      59,    60,    -1,    60,    -1,    61,    48,    39,    -1,    17,
      -1,    18,    -1,    19,    -1,    20,    -1,     3,    48,    63,
      66,    -1,     5,    42,    64,    43,    -1,    -1,    65,    -1,
      -1,    65,    40,    48,    -1,    48,    -1,    44,    67,    45,
      -1,    67,    68,    -1,    -1,     8,    48,    27,    71,    39,
      -1,    10,    48,    27,    71,    39,    -1,    10,    48,    41,
      48,    27,    71,    39,    -1,    10,    49,    41,    48,    27,
      71,    39,    -1,     6,    71,    39,    -1,     7,    71,    39,
      -1,     7,    39,    -1,    48,    42,    81,    43,    39,    -1,
      48,    41,    48,    42,    81,    43,    39,    -1,    23,    39,
      -1,    24,    39,    -1,    69,    -1,    70,    -1,    13,    42,
      71,    43,    66,    -1,    13,    42,    71,    43,    66,    14,
      66,    -1,    15,    42,    71,    43,    66,    -1,    72,    -1,
      72,    29,    73,    -1,    73,    -1,    73,    28,    74,    -1,
      74,    -1,    77,    75,    -1,    75,    76,    77,    -1,    -1,
      34,    -1,    35,    -1,    36,    -1,    37,    -1,    25,    -1,
      26,    -1,    77,    30,    78,    -1,    77,    31,    78,    -1,
      78,    -1,    78,    32,    79,    -1,    78,    33,    79,    -1,
      79,    -1,    31,    79,    -1,    38,    79,    -1,    80,    -1,
      42,    71,    43,    -1,    21,    -1,    22,    -1,    46,    -1,
      47,    -1,    48,    42,    81,    43,    -1,    48,    41,    48,
      42,    81,    43,    -1,    48,    41,    48,    -1,    49,    42,
      81,    43,    -1,    49,    41,    48,    42,    81,    43,    -1,
      49,    41,    48,    -1,    49,    -1,    48,    -1,    82,    -1,
      -1,    82,    40,    71,    -1,    71,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    91,    91,    92,    93,    94,   100,    99,   116,   117,
     121,   122,   133,   138,   139,   143,   154,   158,   165,   166,
     167,   168,   172,   192,   193,   197,   198,   202,   204,   210,
     214,   215,   219,   222,   225,   228,   231,   234,   237,   240,
     243,   246,   248,   251,   252,   257,   259,   265,   271,   275,
     276,   280,   281,   285,   293,   294,   298,   299,   300,   301,
     302,   303,   307,   308,   309,   313,   314,   315,   319,   320,
     321,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   341,   342,   346,   347
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
  "RBRACE", "NUMBER", "STRING_LITERAL", "IDENTIFIER", "THIS", "UMINUS",
  "$accept", "program", "symphony_definition", "$@1", "symphony_members",
  "symphony_member", "ensemble_definition", "ensemble_fields_opt",
  "ensemble_fields", "ensemble_field", "type_spec", "flow_definition",
  "parameter_part", "parameter_list_opt", "parameter_list", "block",
  "statement_list", "statement", "branch_statement", "repeat_statement",
  "expression", "logic_or", "logic_and", "comparison", "comp_tail",
  "compop", "additive", "term", "unary", "primary", "arg_list_opt",
  "arg_list", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    52,    52,    52,    54,    53,    55,    55,
      56,    56,    57,    58,    58,    59,    59,    60,    61,    61,
      61,    61,    62,    63,    63,    64,    64,    65,    65,    66,
      67,    67,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    69,    69,    70,    71,    72,
      72,    73,    73,    74,    75,    75,    76,    76,    76,    76,
      76,    76,    77,    77,    77,    78,    78,    78,    79,    79,
      79,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    81,    81,    82,    82
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     0,     6,     2,     0,
       1,     3,     5,     1,     0,     2,     1,     3,     1,     1,
       1,     1,     4,     4,     0,     1,     0,     3,     1,     3,
       2,     0,     5,     5,     7,     7,     3,     3,     2,     5,
       7,     2,     2,     1,     1,     5,     7,     5,     1,     3,
       1,     3,     1,     2,     3,     0,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     3,     3,     1,     2,     2,
       1,     3,     1,     1,     1,     1,     4,     6,     3,     4,
       6,     3,     1,     1,     1,     0,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     1,     0,     0,     0,     3,     4,     2,    24,
       6,     0,     0,     0,     0,    14,    26,    31,    22,     9,
      18,    19,    20,    21,     0,    13,    16,     0,    28,     0,
      25,     0,     0,    12,    15,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,    30,    43,
      44,     7,     8,     0,    10,    17,    27,    72,    73,     0,
       0,     0,    74,    75,    83,    82,     0,    48,    50,    52,
      55,    64,    67,    70,    38,     0,     0,     0,     0,     0,
       0,    41,    42,     0,    85,     0,    68,    69,     0,     0,
      85,     0,    85,    36,     0,     0,     0,     0,    53,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    87,
       0,    84,    11,    71,    78,     0,    81,     0,    49,    51,
      62,    63,    60,    61,    56,    57,    58,    59,     0,    65,
      66,     0,     0,     0,     0,     0,     0,    85,     0,     0,
      85,    76,    85,    79,    54,    32,    33,     0,     0,    45,
      47,     0,    39,    86,     0,     0,     0,     0,     0,     0,
      77,    80,    34,    35,    46,    40
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     6,    14,    32,    52,     7,    24,    25,    26,
      27,     8,    13,    29,    30,    18,    31,    48,    49,    50,
     109,    67,    68,    69,    98,   128,    70,    71,    72,    73,
     110,   111
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -116
static const yytype_int8 yypact[] =
{
    -116,    33,  -116,   -45,   -41,   -37,  -116,  -116,  -116,     9,
    -116,   -28,   -23,   -22,   -20,    41,    -8,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,     3,    41,  -116,     5,  -116,   -12,
      11,     2,    10,  -116,  -116,    28,  -116,    24,    47,    35,
      40,   -14,    48,    49,    53,    58,  -116,    -3,  -116,  -116,
    -116,  -116,  -116,    50,  -116,  -116,  -116,  -116,  -116,    47,
      47,    47,  -116,  -116,    21,    29,    60,    71,    76,  -116,
      45,    54,  -116,  -116,  -116,    66,    79,    -9,    67,    47,
      47,  -116,  -116,    59,    47,    78,  -116,  -116,    72,    68,
      47,    70,    47,  -116,    47,    47,    47,    47,    77,    47,
      47,  -116,    47,    47,    73,    74,    80,    81,    83,  -116,
      84,    86,  -116,  -116,    87,    85,    88,    89,    76,  -116,
      54,    54,  -116,  -116,  -116,  -116,  -116,  -116,    47,  -116,
    -116,    92,    94,    93,   107,   -22,   -22,    47,    96,    47,
      47,  -116,    47,  -116,    45,  -116,  -116,    47,    47,   105,
    -116,    95,  -116,  -116,    97,    98,   100,   103,   -22,   104,
    -116,  -116,  -116,  -116,  -116,  -116
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,   111,
     112,   113,  -116,  -116,  -116,  -115,  -116,  -116,  -116,  -116,
     -38,  -116,    43,    51,  -116,  -116,    19,   -17,   -54,  -116,
     -88,  -116
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      66,    75,   115,     9,   117,    86,    87,    10,    38,    39,
      40,    11,    41,     3,    12,    42,    15,    43,   103,    16,
     149,   150,    17,    88,    19,    44,    45,    20,    21,    22,
      23,    36,   104,     2,    77,    78,     3,     4,    83,    84,
      28,   106,   107,   164,     5,   129,   130,    46,    33,   151,
      47,    37,   154,    35,   155,    51,    57,    58,    20,    21,
      22,    23,    89,    90,   131,   132,    59,    55,    57,    58,
      91,    92,    56,    60,    74,    96,    97,    61,    59,   120,
     121,    62,    63,    64,    65,    60,    99,   100,    76,    61,
      79,    80,    81,    62,    63,    64,    65,    82,    85,    93,
      94,   153,   122,   123,    95,   101,   102,   108,   105,   156,
     157,   124,   125,   126,   127,   113,   114,   112,   116,   158,
     147,   133,   134,   135,   136,   137,   139,   138,   141,   140,
     142,   145,   143,   146,   148,   152,    34,   118,   159,   162,
     160,   161,   163,   165,    53,    54,   119,   144
};

static const yytype_uint8 yycheck[] =
{
      38,    39,    90,    48,    92,    59,    60,    48,     6,     7,
       8,    48,    10,     3,     5,    13,    44,    15,    27,    42,
     135,   136,    44,    61,    44,    23,    24,    17,    18,    19,
      20,    43,    41,     0,    48,    49,     3,     4,    41,    42,
      48,    79,    80,   158,    11,    99,   100,    45,    45,   137,
      48,    40,   140,    48,   142,    45,    21,    22,    17,    18,
      19,    20,    41,    42,   102,   103,    31,    39,    21,    22,
      41,    42,    48,    38,    39,    30,    31,    42,    31,    96,
      97,    46,    47,    48,    49,    38,    32,    33,    48,    42,
      42,    42,    39,    46,    47,    48,    49,    39,    48,    39,
      29,   139,    25,    26,    28,    39,    27,    48,    41,   147,
     148,    34,    35,    36,    37,    43,    48,    39,    48,    14,
      27,    48,    48,    43,    43,    42,    40,    43,    43,    42,
      42,    39,    43,    39,    27,    39,    25,    94,    43,    39,
      43,    43,    39,    39,    32,    32,    95,   128
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    52,     0,     3,     4,    11,    53,    57,    62,    48,
      48,    48,     5,    63,    54,    44,    42,    44,    66,    44,
      17,    18,    19,    20,    58,    59,    60,    61,    48,    64,
      65,    67,    55,    45,    60,    48,    43,    40,     6,     7,
       8,    10,    13,    15,    23,    24,    45,    48,    68,    69,
      70,    45,    56,    61,    62,    39,    48,    21,    22,    31,
      38,    42,    46,    47,    48,    49,    71,    72,    73,    74,
      77,    78,    79,    80,    39,    71,    48,    48,    49,    42,
      42,    39,    39,    41,    42,    48,    79,    79,    71,    41,
      42,    41,    42,    39,    29,    28,    30,    31,    75,    32,
      33,    39,    27,    27,    41,    41,    71,    71,    48,    71,
      81,    82,    39,    43,    48,    81,    48,    81,    73,    74,
      78,    78,    25,    26,    34,    35,    36,    37,    76,    79,
      79,    71,    71,    48,    48,    43,    43,    42,    43,    40,
      42,    43,    42,    43,    77,    39,    39,    27,    27,    66,
      66,    81,    39,    71,    81,    81,    71,    71,    14,    43,
      43,    43,    39,    39,    66,    39
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
#line 100 "orchestra.y"
    {
      /* Enter class/ensemble scope for name qualification. */
      g_current_class = (yyvsp[(2) - (2)].sval);
      g_current_symphony_fields = NULL;
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 106 "orchestra.y"
    {
      /* Make symphony constructible (constructor + fields), like a class with data. */
      register_ensemble((yyvsp[(2) - (6)].sval), g_current_symphony_fields);
      g_current_symphony_fields = NULL;
      /* Exit class/ensemble scope. */
      g_current_class = NULL;
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 123 "orchestra.y"
    {
        g_current_symphony_fields = fieldlist_append(g_current_symphony_fields, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval));
      ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 134 "orchestra.y"
    { register_ensemble((yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].field_list)); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 139 "orchestra.y"
    { (yyval.field_list) = NULL; ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 144 "orchestra.y"
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

  case 16:

/* Line 1455 of yacc.c  */
#line 154 "orchestra.y"
    { (yyval.field_list) = (yyvsp[(1) - (1)].field_list); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 159 "orchestra.y"
    {
        (yyval.field_list) = fieldlist_append(NULL, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval));
      ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 165 "orchestra.y"
    { (yyval.sval) = arena_strdup("int"); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 166 "orchestra.y"
    { (yyval.sval) = arena_strdup("float"); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 167 "orchestra.y"
    { (yyval.sval) = arena_strdup("bool"); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 168 "orchestra.y"
    { (yyval.sval) = arena_strdup("string"); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 173 "orchestra.y"
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

  case 23:

/* Line 1455 of yacc.c  */
#line 192 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(3) - (4)].name_list); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 193 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 197 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(1) - (1)].name_list); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 198 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 203 "orchestra.y"
    { (yyval.name_list) = namelist_append((yyvsp[(1) - (3)].name_list), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 205 "orchestra.y"
    { (yyval.name_list) = namelist_append(NULL, (yyvsp[(1) - (1)].sval)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 210 "orchestra.y"
    { (yyval.stmt) = make_block((yyvsp[(2) - (3)].stmt_list)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 214 "orchestra.y"
    { (yyval.stmt_list) = stmtlist_append((yyvsp[(1) - (2)].stmt_list), (yyvsp[(2) - (2)].stmt)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 215 "orchestra.y"
    { (yyval.stmt_list) = NULL; ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 220 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_NOTE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 223 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_STAGE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 226 "orchestra.y"
    { (yyval.stmt) = make_field_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].sval), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 229 "orchestra.y"
    { (yyval.stmt) = make_field_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].sval), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 232 "orchestra.y"
    { (yyval.stmt) = make_emit((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 235 "orchestra.y"
    { (yyval.stmt) = make_return((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 238 "orchestra.y"
    { (yyval.stmt) = make_return(NULL); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 241 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_call((yyvsp[(1) - (5)].sval), (yyvsp[(3) - (5)].expr_list))); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 244 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_dotcall((yyvsp[(1) - (7)].sval), (yyvsp[(3) - (7)].sval), (yyvsp[(5) - (7)].expr_list))); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 247 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_BREAK, NULL, NULL); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 249 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_CONTINUE, NULL, NULL); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 258 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt), NULL); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 260 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt)); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 266 "orchestra.y"
    { (yyval.stmt) = make_repeat((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 271 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 275 "orchestra.y"
    { (yyval.expr) = make_bin(OP_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 276 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 280 "orchestra.y"
    { (yyval.expr) = make_bin(OP_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 281 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 286 "orchestra.y"
    {
        if ((yyvsp[(2) - (2)].chain_parts)) (yyval.expr) = make_chain((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].chain_parts));
        else (yyval.expr) = (yyvsp[(1) - (2)].expr);
      ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 293 "orchestra.y"
    { (yyval.chain_parts) = chainpart_append((yyvsp[(1) - (3)].chain_parts), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 294 "orchestra.y"
    { (yyval.chain_parts) = NULL; ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 298 "orchestra.y"
    { (yyval.ival) = OP_LT; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 299 "orchestra.y"
    { (yyval.ival) = OP_LE; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 300 "orchestra.y"
    { (yyval.ival) = OP_GT; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 301 "orchestra.y"
    { (yyval.ival) = OP_GE; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 302 "orchestra.y"
    { (yyval.ival) = OP_EQ; ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 303 "orchestra.y"
    { (yyval.ival) = OP_NE; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 307 "orchestra.y"
    { (yyval.expr) = make_bin(OP_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 308 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 309 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 313 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MUL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 314 "orchestra.y"
    { (yyval.expr) = make_bin(OP_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 315 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 319 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NEG, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 320 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NOT, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 321 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 325 "orchestra.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 326 "orchestra.y"
    { (yyval.expr) = make_lit_bool(1); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 327 "orchestra.y"
    { (yyval.expr) = make_lit_bool(0); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 328 "orchestra.y"
    { (yyval.expr) = make_lit_num((yyvsp[(1) - (1)].numlit).num, (yyvsp[(1) - (1)].numlit).is_float); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 329 "orchestra.y"
    { (yyval.expr) = make_lit_string((yyvsp[(1) - (1)].sval)); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 330 "orchestra.y"
    { (yyval.expr) = make_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 331 "orchestra.y"
    { (yyval.expr) = make_dotcall((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 332 "orchestra.y"
    { (yyval.expr) = make_field((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 333 "orchestra.y"
    { (yyval.expr) = make_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 334 "orchestra.y"
    { (yyval.expr) = make_dotcall((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 335 "orchestra.y"
    { (yyval.expr) = make_field((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 336 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 337 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 341 "orchestra.y"
    { (yyval.expr_list) = (yyvsp[(1) - (1)].expr_list); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 342 "orchestra.y"
    { (yyval.expr_list) = NULL; ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 346 "orchestra.y"
    { (yyval.expr_list) = exprlist_append((yyvsp[(1) - (3)].expr_list), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 347 "orchestra.y"
    { (yyval.expr_list) = exprlist_append(NULL, (yyvsp[(1) - (1)].expr)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2147 "orchestra.tab.c"
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
#line 350 "orchestra.y"


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
