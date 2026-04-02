
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
extern int yylineno;

int yylex();
int yyerror(const char *s);

#include "symbol_table.h"

#include "interpreter.h"

#include "cpp_printer.h"

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
#line 105 "orchestra.tab.c"

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
#line 32 "orchestra.y"

  struct Expr;
  struct Stmt;
  struct StmtList;
  struct ExprList;
  struct NameList;
  struct ChainPart;



/* Line 209 of yacc.c  */
#line 140 "orchestra.tab.c"

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
     DEREF = 272,
     STAGETHRU = 273,
     AMPERSAND = 274,
     EXTENDS = 275,
     TYPE_INT = 276,
     TYPE_FLOAT = 277,
     TYPE_BOOL = 278,
     TYPE_STRING = 279,
     TRUE = 280,
     FALSE = 281,
     BREAK = 282,
     CONTINUE = 283,
     EQ = 284,
     NE = 285,
     ASSIGN = 286,
     AND = 287,
     OR = 288,
     PLUS = 289,
     MINUS = 290,
     MUL = 291,
     DIV = 292,
     LT = 293,
     LE = 294,
     GT = 295,
     GE = 296,
     NOT = 297,
     SEMICOLON = 298,
     COMMA = 299,
     DOT = 300,
     LPAREN = 301,
     RPAREN = 302,
     LBRACE = 303,
     RBRACE = 304,
     LBRACKET = 305,
     RBRACKET = 306,
     NUMBER = 307,
     STRING_LITERAL = 308,
     IDENTIFIER = 309,
     THIS = 310,
     SUPER = 311,
     UMINUS = 312
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 41 "orchestra.y"

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
#line 229 "orchestra.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 241 "orchestra.tab.c"

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
#define YYLAST   247

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  110
/* YYNRULES -- Number of states.  */
#define YYNSTATES  246

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   312

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    13,    14,    22,    25,
      26,    29,    30,    32,    36,    42,    44,    45,    48,    50,
      54,    56,    58,    60,    62,    67,    72,    73,    75,    76,
      80,    82,    86,    89,    90,    96,   102,   108,   117,   125,
     133,   139,   143,   147,   151,   154,   160,   168,   171,   174,
     180,   182,   184,   186,   188,   194,   202,   208,   218,   227,
     232,   237,   238,   243,   251,   252,   254,   258,   260,   264,
     266,   269,   273,   274,   276,   278,   280,   282,   284,   286,
     290,   294,   296,   300,   304,   306,   309,   312,   314,   318,
     322,   324,   326,   328,   330,   335,   340,   347,   351,   356,
     363,   367,   369,   376,   381,   384,   389,   391,   393,   394,
     398
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      59,     0,    -1,    59,    70,    -1,    59,    60,    -1,    59,
      65,    -1,    -1,    -1,     4,    54,    62,    61,    48,    63,
      49,    -1,    20,    54,    -1,    -1,    63,    64,    -1,    -1,
      70,    -1,    69,    54,    43,    -1,    11,    54,    48,    66,
      49,    -1,    67,    -1,    -1,    67,    68,    -1,    68,    -1,
      69,    54,    43,    -1,    21,    -1,    22,    -1,    23,    -1,
      24,    -1,     3,    54,    71,    74,    -1,     5,    46,    72,
      47,    -1,    -1,    73,    -1,    -1,    73,    44,    54,    -1,
      54,    -1,    48,    75,    49,    -1,    75,    76,    -1,    -1,
       8,    54,    31,    82,    43,    -1,     9,    54,    31,    82,
      43,    -1,    10,    54,    31,    82,    43,    -1,    10,    54,
      50,    82,    51,    31,    82,    43,    -1,    10,    54,    45,
      54,    31,    82,    43,    -1,    10,    55,    45,    54,    31,
      82,    43,    -1,    56,    46,    92,    47,    43,    -1,     6,
      82,    43,    -1,    12,    93,    43,    -1,     7,    82,    43,
      -1,     7,    43,    -1,    54,    46,    92,    47,    43,    -1,
      54,    45,    54,    46,    92,    47,    43,    -1,    27,    43,
      -1,    28,    43,    -1,    18,    54,    31,    82,    43,    -1,
      74,    -1,    77,    -1,    78,    -1,    79,    -1,    13,    46,
      82,    47,    74,    -1,    13,    46,    82,    47,    74,    14,
      74,    -1,    15,    46,    82,    47,    74,    -1,    16,    46,
      80,    43,    82,    43,    81,    47,    74,    -1,    16,    46,
      80,    43,    43,    81,    47,    74,    -1,     8,    54,    31,
      82,    -1,    10,    54,    31,    82,    -1,    -1,    10,    54,
      31,    82,    -1,    10,    54,    50,    82,    51,    31,    82,
      -1,    -1,    83,    -1,    83,    33,    84,    -1,    84,    -1,
      84,    32,    85,    -1,    85,    -1,    88,    86,    -1,    86,
      87,    88,    -1,    -1,    38,    -1,    39,    -1,    40,    -1,
      41,    -1,    29,    -1,    30,    -1,    88,    34,    89,    -1,
      88,    35,    89,    -1,    89,    -1,    89,    36,    90,    -1,
      89,    37,    90,    -1,    90,    -1,    35,    90,    -1,    42,
      90,    -1,    91,    -1,    46,    82,    47,    -1,    50,    92,
      51,    -1,    25,    -1,    26,    -1,    52,    -1,    53,    -1,
      54,    50,    82,    51,    -1,    54,    46,    92,    47,    -1,
      54,    45,    54,    46,    92,    47,    -1,    54,    45,    54,
      -1,    55,    46,    92,    47,    -1,    55,    45,    54,    46,
      92,    47,    -1,    55,    45,    54,    -1,    55,    -1,    56,
      45,    54,    46,    92,    47,    -1,    56,    46,    92,    47,
      -1,    19,    54,    -1,    17,    46,    82,    47,    -1,    54,
      -1,    93,    -1,    -1,    93,    44,    82,    -1,    82,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   100,   100,   101,   102,   103,   109,   108,   126,   127,
     131,   132,   136,   137,   148,   153,   154,   158,   169,   173,
     180,   181,   182,   183,   187,   207,   208,   212,   213,   217,
     219,   225,   229,   230,   234,   237,   240,   243,   246,   249,
     252,   255,   258,   261,   264,   267,   270,   273,   275,   278,
     281,   283,   284,   285,   290,   292,   298,   306,   308,   313,
     315,   318,   322,   324,   327,   332,   336,   337,   341,   342,
     346,   354,   355,   359,   360,   361,   362,   363,   364,   368,
     369,   370,   374,   375,   376,   380,   381,   382,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   401,   402,   403,   404,   405,   409,   410,   414,
     415
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FLOW", "SYMPHONY", "TAKE", "EMIT",
  "RETURN", "NOTE", "FIXED", "STAGE", "ENSEMBLE", "PLAY", "BRANCH",
  "ELSEWISE", "REPEAT", "SCORE", "DEREF", "STAGETHRU", "AMPERSAND",
  "EXTENDS", "TYPE_INT", "TYPE_FLOAT", "TYPE_BOOL", "TYPE_STRING", "TRUE",
  "FALSE", "BREAK", "CONTINUE", "EQ", "NE", "ASSIGN", "AND", "OR", "PLUS",
  "MINUS", "MUL", "DIV", "LT", "LE", "GT", "GE", "NOT", "SEMICOLON",
  "COMMA", "DOT", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET",
  "RBRACKET", "NUMBER", "STRING_LITERAL", "IDENTIFIER", "THIS", "SUPER",
  "UMINUS", "$accept", "program", "symphony_definition", "$@1",
  "extends_opt", "symphony_members", "symphony_member",
  "ensemble_definition", "ensemble_fields_opt", "ensemble_fields",
  "ensemble_field", "type_spec", "flow_definition", "parameter_part",
  "parameter_list_opt", "parameter_list", "block", "statement_list",
  "statement", "branch_statement", "repeat_statement", "score_statement",
  "score_init", "score_step", "expression", "logic_or", "logic_and",
  "comparison", "comp_tail", "compop", "additive", "term", "unary",
  "primary", "arg_list_opt", "arg_list", 0
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
     305,   306,   307,   308,   309,   310,   311,   312
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    59,    59,    59,    61,    60,    62,    62,
      63,    63,    64,    64,    65,    66,    66,    67,    67,    68,
      69,    69,    69,    69,    70,    71,    71,    72,    72,    73,
      73,    74,    75,    75,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    77,    77,    78,    79,    79,    80,
      80,    80,    81,    81,    81,    82,    83,    83,    84,    84,
      85,    86,    86,    87,    87,    87,    87,    87,    87,    88,
      88,    88,    89,    89,    89,    90,    90,    90,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    92,    92,    93,
      93
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     0,     7,     2,     0,
       2,     0,     1,     3,     5,     1,     0,     2,     1,     3,
       1,     1,     1,     1,     4,     4,     0,     1,     0,     3,
       1,     3,     2,     0,     5,     5,     5,     8,     7,     7,
       5,     3,     3,     3,     2,     5,     7,     2,     2,     5,
       1,     1,     1,     1,     5,     7,     5,     9,     8,     4,
       4,     0,     4,     7,     0,     1,     3,     1,     3,     1,
       2,     3,     0,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     3,     3,     1,     2,     2,     1,     3,     3,
       1,     1,     1,     1,     4,     4,     6,     3,     4,     6,
       3,     1,     6,     4,     2,     4,     1,     1,     0,     3,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     1,     0,     0,     0,     3,     4,     2,    26,
       9,     0,     0,     0,     0,     6,    16,    28,    33,    24,
       8,     0,    20,    21,    22,    23,     0,    15,    18,     0,
      30,     0,    27,     0,    11,    14,    17,     0,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,     0,     0,    50,    32,    51,    52,    53,
       0,    19,    29,     0,     0,    90,    91,     0,     0,     0,
     108,    92,    93,   106,   101,     0,     0,    65,    67,    69,
      72,    81,    84,    87,    44,     0,     0,     0,     0,     0,
     110,     0,     0,     0,    61,     0,    47,    48,     0,   108,
     108,     7,    10,     0,    12,     0,   104,    85,    86,     0,
       0,   107,     0,   108,     0,     0,   108,     0,   108,    41,
       0,     0,     0,     0,    70,     0,     0,    43,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    97,
       0,     0,   100,     0,     0,     0,    66,    68,    79,    80,
      77,    78,    73,    74,    75,    76,     0,    82,    83,     0,
       0,     0,     0,     0,     0,   109,     0,     0,     0,     0,
       0,     0,   108,     0,     0,    13,   105,   108,    95,    94,
     108,    98,   108,   103,    71,    34,    35,    36,     0,     0,
       0,    54,    56,     0,     0,    64,     0,    49,     0,    45,
      40,     0,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,    64,     0,    96,    99,   102,    38,     0,    39,
      55,     0,     0,     0,    46,    37,     0,     0,    58,     0,
      62,     0,    57,     0,     0,    63
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     6,    21,    15,    60,   102,     7,    26,    27,
      28,    29,     8,    13,    31,    32,    19,    33,    56,    57,
      58,    59,   140,   221,    90,    77,    78,    79,   124,   166,
      80,    81,    82,    83,   110,   111
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -96
static const yytype_int16 yypact[] =
{
     -96,     9,   -96,   -21,   -12,    -9,   -96,   -96,   -96,    14,
      43,   -22,    23,    35,    20,   -96,    26,    34,   -96,   -96,
     -96,    38,   -96,   -96,   -96,   -96,    45,    26,   -96,    47,
     -96,    52,    61,    28,   -96,   -96,   -96,    65,   -96,    55,
     169,    81,    56,    57,   -30,   169,    66,    67,    68,    63,
      72,    75,   -96,   -17,    73,   -96,   -96,   -96,   -96,   -96,
      36,   -96,   -96,    74,    71,   -96,   -96,   169,   169,   169,
     169,   -96,   -96,   -35,   -14,    22,    78,    89,    94,   -96,
      17,    42,   -96,   -96,   -96,    85,    98,    99,   -28,    87,
     -96,    37,   169,   169,     6,   107,   -96,   -96,    86,   169,
     169,   -96,   -96,    88,   -96,   169,   -96,   -96,   -96,    92,
      95,   101,    93,   169,   169,    96,   169,    97,   169,   -96,
     169,   169,   169,   169,    32,   169,   169,   -96,   169,   169,
     169,   103,   169,   104,   -96,   169,   102,   105,   106,   112,
     110,   169,   109,   120,   121,   126,   123,   -96,   -96,   125,
     127,   122,   129,   130,   132,   134,    94,   -96,    42,    42,
     -96,   -96,   -96,   -96,   -96,   -96,   169,   -96,   -96,   139,
     142,   153,   117,   149,   170,   -96,    35,    35,   171,   172,
     137,   164,   169,   165,   166,   -96,   -96,   169,   -96,   -96,
     169,   -96,   169,   -96,    17,   -96,   -96,   -96,   169,   179,
     169,   198,   -96,   169,   169,   203,   173,   -96,   167,   -96,
     -96,   180,   181,   182,   174,   169,   175,    35,   -96,   -96,
     176,   184,   203,   177,   -96,   -96,   -96,   -96,   183,   -96,
     -96,   -23,    35,   185,   -96,   -96,   169,   169,   -96,    35,
     -96,   186,   -96,   202,   169,   -96
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     207,   178,   187,   -96,   -96,   -96,   -33,   -96,   -96,   -96,
     -96,   -96,   -96,    13,   -39,   -96,   116,   118,   -96,   -96,
      76,   -19,   -61,   -96,   -95,   195
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      55,    76,    85,   130,   143,   144,   107,   108,   236,     2,
     112,   113,     3,     4,   138,   114,   139,   131,   150,    12,
       5,   153,   132,   155,    88,    89,    16,   237,    98,    99,
     109,   115,   116,     9,    40,    41,    42,    43,    44,     3,
      45,    46,    10,    47,    48,    11,    49,    22,    23,    24,
      25,   122,   123,   136,   137,    50,    51,    22,    23,    24,
      25,   160,   161,    14,   167,   168,   146,   117,   118,    17,
     162,   163,   164,   165,    20,   151,    18,    52,   125,   126,
     134,   135,    53,    18,    54,   101,    34,   208,    30,   169,
     170,   171,   211,   173,    35,   212,   175,   213,    63,    38,
      64,    37,   181,   158,   159,    39,    65,    66,    61,    62,
      86,    87,    92,    93,    94,    96,    67,    95,    97,   100,
     105,   119,   120,    68,    84,   106,   121,    69,   127,   128,
     129,    70,   133,    71,    72,    73,    74,    75,   141,   147,
     142,   206,   145,   201,   202,   135,   148,   149,   198,   176,
     152,   154,   177,   180,    63,   182,    64,   172,   174,   214,
     178,   216,    65,    66,   218,   219,   179,   183,   184,   185,
     186,   187,    67,   189,   188,   190,   228,   191,   192,    68,
     205,   193,   195,    69,   230,   196,    63,    70,    64,    71,
      72,    73,    74,    75,    65,    66,   197,   240,   241,   238,
     199,   200,   203,   204,    67,   245,   242,   207,   209,   210,
     215,    68,   217,   220,   223,    69,   222,   227,   229,    70,
     234,    71,    72,    73,    74,    75,   235,   224,   225,   226,
     231,   232,   239,   244,    36,   233,   156,   243,   103,   157,
      91,     0,   194,     0,     0,     0,     0,   104
};

static const yytype_int16 yycheck[] =
{
      33,    40,    41,    31,    99,   100,    67,    68,    31,     0,
      45,    46,     3,     4,     8,    50,    10,    45,   113,     5,
      11,   116,    50,   118,    54,    55,    48,    50,    45,    46,
      69,    45,    46,    54,     6,     7,     8,     9,    10,     3,
      12,    13,    54,    15,    16,    54,    18,    21,    22,    23,
      24,    34,    35,    92,    93,    27,    28,    21,    22,    23,
      24,    29,    30,    20,   125,   126,   105,    45,    46,    46,
      38,    39,    40,    41,    54,   114,    48,    49,    36,    37,
      43,    44,    54,    48,    56,    49,    48,   182,    54,   128,
     129,   130,   187,   132,    49,   190,   135,   192,    17,    47,
      19,    54,   141,   122,   123,    44,    25,    26,    43,    54,
      54,    54,    46,    46,    46,    43,    35,    54,    43,    46,
      46,    43,    33,    42,    43,    54,    32,    46,    43,    31,
      31,    50,    45,    52,    53,    54,    55,    56,    31,    47,
      54,   180,    54,   176,   177,    44,    51,    54,    31,    47,
      54,    54,    47,    43,    17,    46,    19,    54,    54,   198,
      54,   200,    25,    26,   203,   204,    54,    47,    47,    43,
      47,    46,    35,    51,    47,    46,   215,    47,    46,    42,
      43,    47,    43,    46,   217,    43,    17,    50,    19,    52,
      53,    54,    55,    56,    25,    26,    43,   236,   237,   232,
      51,    31,    31,    31,    35,   244,   239,    43,    43,    43,
      31,    42,    14,    10,    47,    46,    43,    43,    43,    50,
      43,    52,    53,    54,    55,    56,    43,    47,    47,    47,
      54,    47,    47,    31,    27,   222,   120,    51,    60,   121,
      45,    -1,   166,    -1,    -1,    -1,    -1,    60
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    59,     0,     3,     4,    11,    60,    65,    70,    54,
      54,    54,     5,    71,    20,    62,    48,    46,    48,    74,
      54,    61,    21,    22,    23,    24,    66,    67,    68,    69,
      54,    72,    73,    75,    48,    49,    68,    54,    47,    44,
       6,     7,     8,     9,    10,    12,    13,    15,    16,    18,
      27,    28,    49,    54,    56,    74,    76,    77,    78,    79,
      63,    43,    54,    17,    19,    25,    26,    35,    42,    46,
      50,    52,    53,    54,    55,    56,    82,    83,    84,    85,
      88,    89,    90,    91,    43,    82,    54,    54,    54,    55,
      82,    93,    46,    46,    46,    54,    43,    43,    45,    46,
      46,    49,    64,    69,    70,    46,    54,    90,    90,    82,
      92,    93,    45,    46,    50,    45,    46,    45,    46,    43,
      33,    32,    34,    35,    86,    36,    37,    43,    31,    31,
      31,    45,    50,    45,    43,    44,    82,    82,     8,    10,
      80,    31,    54,    92,    92,    54,    82,    47,    51,    54,
      92,    82,    54,    92,    54,    92,    84,    85,    89,    89,
      29,    30,    38,    39,    40,    41,    87,    90,    90,    82,
      82,    82,    54,    82,    54,    82,    47,    47,    54,    54,
      43,    82,    46,    47,    47,    43,    47,    46,    47,    51,
      46,    47,    46,    47,    88,    43,    43,    43,    31,    51,
      31,    74,    74,    31,    31,    43,    82,    43,    92,    43,
      43,    92,    92,    92,    82,    31,    82,    14,    82,    82,
      10,    81,    43,    47,    47,    47,    47,    43,    82,    43,
      74,    54,    47,    81,    43,    43,    31,    50,    74,    47,
      82,    82,    74,    51,    31,    82
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
#line 109 "orchestra.y"
    {
      /* Enter class/ensemble scope for name qualification. */
      g_current_class = (yyvsp[(2) - (3)].sval);
      g_current_symphony_fields = NULL;
      /* stash parent name in a simple way: store it in g_current_symphony_fields later via register */
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 116 "orchestra.y"
    {
      /* Make symphony constructible (constructor + fields), like a class with data. */
      register_ensemble((yyvsp[(2) - (7)].sval), (yyvsp[(3) - (7)].sval), g_current_symphony_fields);
      g_current_symphony_fields = NULL;
      /* Exit class/ensemble scope. */
      g_current_class = NULL;
    ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 126 "orchestra.y"
    { (yyval.sval) = (yyvsp[(2) - (2)].sval); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 127 "orchestra.y"
    { (yyval.sval) = NULL; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 138 "orchestra.y"
    {
        g_current_symphony_fields = fieldlist_append(g_current_symphony_fields, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval));
      ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 149 "orchestra.y"
    { register_ensemble((yyvsp[(2) - (5)].sval), NULL, (yyvsp[(4) - (5)].field_list)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 154 "orchestra.y"
    { (yyval.field_list) = NULL; ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 159 "orchestra.y"
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

  case 18:

/* Line 1455 of yacc.c  */
#line 169 "orchestra.y"
    { (yyval.field_list) = (yyvsp[(1) - (1)].field_list); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 174 "orchestra.y"
    {
        (yyval.field_list) = fieldlist_append(NULL, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval));
      ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 180 "orchestra.y"
    { (yyval.sval) = arena_strdup("int"); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 181 "orchestra.y"
    { (yyval.sval) = arena_strdup("float"); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 182 "orchestra.y"
    { (yyval.sval) = arena_strdup("bool"); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 183 "orchestra.y"
    { (yyval.sval) = arena_strdup("string"); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 188 "orchestra.y"
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

  case 25:

/* Line 1455 of yacc.c  */
#line 207 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(3) - (4)].name_list); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 208 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 212 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(1) - (1)].name_list); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 213 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 218 "orchestra.y"
    { (yyval.name_list) = namelist_append((yyvsp[(1) - (3)].name_list), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 220 "orchestra.y"
    { (yyval.name_list) = namelist_append(NULL, (yyvsp[(1) - (1)].sval)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 225 "orchestra.y"
    { (yyval.stmt) = make_block((yyvsp[(2) - (3)].stmt_list)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 229 "orchestra.y"
    { (yyval.stmt_list) = stmtlist_append((yyvsp[(1) - (2)].stmt_list), (yyvsp[(2) - (2)].stmt)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 230 "orchestra.y"
    { (yyval.stmt_list) = NULL; ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 235 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_NOTE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 238 "orchestra.y"
    { (yyval.stmt) = make_fixed((yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 241 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_STAGE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 244 "orchestra.y"
    { (yyval.stmt) = make_index_stage((yyvsp[(2) - (8)].sval), (yyvsp[(4) - (8)].expr), (yyvsp[(7) - (8)].expr)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 247 "orchestra.y"
    { (yyval.stmt) = make_field_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].sval), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 250 "orchestra.y"
    { (yyval.stmt) = make_field_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].sval), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 253 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_superctor((yyvsp[(3) - (5)].expr_list))); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 256 "orchestra.y"
    { (yyval.stmt) = make_emit((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 259 "orchestra.y"
    { (yyval.stmt) = make_play((yyvsp[(2) - (3)].expr_list)); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 262 "orchestra.y"
    { (yyval.stmt) = make_return((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 265 "orchestra.y"
    { (yyval.stmt) = make_return(NULL); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 268 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_call((yyvsp[(1) - (5)].sval), (yyvsp[(3) - (5)].expr_list))); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 271 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_dotcall((yyvsp[(1) - (7)].sval), (yyvsp[(3) - (7)].sval), (yyvsp[(5) - (7)].expr_list))); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 274 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_BREAK, NULL, NULL); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 276 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_CONTINUE, NULL, NULL); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 279 "orchestra.y"
    { (yyval.stmt) = make_stagethru((yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 291 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt), NULL); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 293 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt)); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 299 "orchestra.y"
    { (yyval.stmt) = make_repeat((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 307 "orchestra.y"
    { (yyval.stmt) = make_score((yyvsp[(3) - (9)].stmt), (yyvsp[(5) - (9)].expr), (yyvsp[(7) - (9)].stmt), (yyvsp[(9) - (9)].stmt)); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 309 "orchestra.y"
    { (yyval.stmt) = make_score((yyvsp[(3) - (8)].stmt), NULL, (yyvsp[(6) - (8)].stmt), (yyvsp[(8) - (8)].stmt)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 314 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_NOTE, (yyvsp[(2) - (4)].sval), (yyvsp[(4) - (4)].expr)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 316 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_STAGE, (yyvsp[(2) - (4)].sval), (yyvsp[(4) - (4)].expr)); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 318 "orchestra.y"
    { (yyval.stmt) = NULL; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 323 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_STAGE, (yyvsp[(2) - (4)].sval), (yyvsp[(4) - (4)].expr)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 325 "orchestra.y"
    { (yyval.stmt) = make_index_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].expr), (yyvsp[(7) - (7)].expr)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 327 "orchestra.y"
    { (yyval.stmt) = NULL; ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 332 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 336 "orchestra.y"
    { (yyval.expr) = make_bin(OP_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 337 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 341 "orchestra.y"
    { (yyval.expr) = make_bin(OP_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 342 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 347 "orchestra.y"
    {
        if ((yyvsp[(2) - (2)].chain_parts)) (yyval.expr) = make_chain((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].chain_parts));
        else (yyval.expr) = (yyvsp[(1) - (2)].expr);
      ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 354 "orchestra.y"
    { (yyval.chain_parts) = chainpart_append((yyvsp[(1) - (3)].chain_parts), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 355 "orchestra.y"
    { (yyval.chain_parts) = NULL; ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 359 "orchestra.y"
    { (yyval.ival) = OP_LT; ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 360 "orchestra.y"
    { (yyval.ival) = OP_LE; ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 361 "orchestra.y"
    { (yyval.ival) = OP_GT; ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 362 "orchestra.y"
    { (yyval.ival) = OP_GE; ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 363 "orchestra.y"
    { (yyval.ival) = OP_EQ; ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 364 "orchestra.y"
    { (yyval.ival) = OP_NE; ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 368 "orchestra.y"
    { (yyval.expr) = make_bin(OP_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 369 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 370 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 374 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MUL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 375 "orchestra.y"
    { (yyval.expr) = make_bin(OP_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 376 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 380 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NEG, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 381 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NOT, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 382 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 386 "orchestra.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 387 "orchestra.y"
    { (yyval.expr) = make_arraylit((yyvsp[(2) - (3)].expr_list)); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 388 "orchestra.y"
    { (yyval.expr) = make_lit_bool(1); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 389 "orchestra.y"
    { (yyval.expr) = make_lit_bool(0); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 390 "orchestra.y"
    { (yyval.expr) = make_lit_num((yyvsp[(1) - (1)].numlit).num, (yyvsp[(1) - (1)].numlit).is_float); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 391 "orchestra.y"
    { (yyval.expr) = make_lit_string((yyvsp[(1) - (1)].sval)); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 392 "orchestra.y"
    { (yyval.expr) = make_index(make_var((yyvsp[(1) - (4)].sval)), (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 393 "orchestra.y"
    { (yyval.expr) = make_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 394 "orchestra.y"
    { (yyval.expr) = make_dotcall((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 395 "orchestra.y"
    { (yyval.expr) = make_field((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 396 "orchestra.y"
    { (yyval.expr) = make_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 397 "orchestra.y"
    { (yyval.expr) = make_dotcall((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 398 "orchestra.y"
    { (yyval.expr) = make_field((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 399 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 401 "orchestra.y"
    { (yyval.expr) = make_supercall((yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 402 "orchestra.y"
    { (yyval.expr) = make_superctor((yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 403 "orchestra.y"
    { (yyval.expr) = make_addrof((yyvsp[(2) - (2)].sval)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 404 "orchestra.y"
    { (yyval.expr) = make_deref((yyvsp[(3) - (4)].expr)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 405 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 409 "orchestra.y"
    { (yyval.expr_list) = (yyvsp[(1) - (1)].expr_list); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 410 "orchestra.y"
    { (yyval.expr_list) = NULL; ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 414 "orchestra.y"
    { (yyval.expr_list) = exprlist_append((yyvsp[(1) - (3)].expr_list), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 415 "orchestra.y"
    { (yyval.expr_list) = exprlist_append(NULL, (yyvsp[(1) - (1)].expr)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2375 "orchestra.tab.c"
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
#line 418 "orchestra.y"


int main(int argc, char *argv[]) {
  const char *backend = "ast";
  const char *emit = NULL;
  const char *emit_style = "cpp";

  /* minimal CLI:
      orchestra.exe input.txt output.txt [--backend=ast|vm]
      orchestra.exe input.txt output.txt --emit=cpp
      orchestra.exe input.txt output.txt --emit cpp
      orchestra.exe input.txt output.txt --emit=bytecode
      orchestra.exe input.txt output.txt --emit=cpp --emit-style=cpp|pseudo
  */
  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], "--backend=vm") == 0) backend = "vm";
    else if (strcmp(argv[i], "--backend=ast") == 0) backend = "ast";
    else if (strcmp(argv[i], "--backend") == 0 && i + 1 < argc) backend = argv[++i];
    else if (strcmp(argv[i], "--emit=cpp") == 0) emit = "cpp";
    else if (strcmp(argv[i], "--emit=bytecode") == 0) emit = "bytecode";
    else if (strcmp(argv[i], "--emit") == 0 && i + 1 < argc) emit = argv[++i];
    else if (strcmp(argv[i], "--emit-style=cpp") == 0) emit_style = "cpp";
    else if (strcmp(argv[i], "--emit-style=pseudo") == 0) emit_style = "pseudo";
    else if (strcmp(argv[i], "--emit-style") == 0 && i + 1 < argc) emit_style = argv[++i];
  }

  if (argc < 3) {
    printf("Usage: %s input.txt output.txt [--backend=ast|vm] [--emit=cpp|bytecode] [--emit-style=cpp|pseudo]\n", argv[0]);
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
      if (emit && strcmp(emit, "cpp") == 0) {
        OrchCppEmitStyle style = ORCH_CPP_STYLE_CPP;
        if (emit_style && strcmp(emit_style, "pseudo") == 0) style = ORCH_CPP_STYLE_PSEUDO;
        dump_cpp_program_with_style(g_main_block, yyout, style);
      } else if (emit && strcmp(emit, "bytecode") == 0) {
        extern int dump_bytecode_vm(struct Stmt *root, FILE *out);
        dump_bytecode_vm(g_main_block, yyout);
      } else if (strcmp(backend, "vm") == 0) {
        extern int execute_program_vm(struct Stmt *root, FILE *out);
        execute_program_vm(g_main_block, yyout);
      } else {
        execute_program(g_main_block, yyout);
      }
      g_main_block = NULL;
    }

    free_all_flows();
    arena_free_all();

    fclose(input);
    fclose(output);
    return 0;
}

int yyerror(const char *s) {
  fprintf(yyout ? yyout : stdout, "Syntax Error: %s at line %d\n", s, yylineno);
    return 0;
}
