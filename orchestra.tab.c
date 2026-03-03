
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
     EXTENDS = 272,
     TYPE_INT = 273,
     TYPE_FLOAT = 274,
     TYPE_BOOL = 275,
     TYPE_STRING = 276,
     TRUE = 277,
     FALSE = 278,
     BREAK = 279,
     CONTINUE = 280,
     EQ = 281,
     NE = 282,
     ASSIGN = 283,
     AND = 284,
     OR = 285,
     PLUS = 286,
     MINUS = 287,
     MUL = 288,
     DIV = 289,
     LT = 290,
     LE = 291,
     GT = 292,
     GE = 293,
     NOT = 294,
     SEMICOLON = 295,
     COMMA = 296,
     DOT = 297,
     LPAREN = 298,
     RPAREN = 299,
     LBRACE = 300,
     RBRACE = 301,
     LBRACKET = 302,
     RBRACKET = 303,
     NUMBER = 304,
     STRING_LITERAL = 305,
     IDENTIFIER = 306,
     THIS = 307,
     SUPER = 308,
     UMINUS = 309
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
#line 223 "orchestra.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 235 "orchestra.tab.c"

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
#define YYLAST   183

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNRULES -- Number of states.  */
#define YYNSTATES  196

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   309

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    13,    14,    22,    25,
      26,    29,    30,    32,    36,    42,    44,    45,    48,    50,
      54,    56,    58,    60,    62,    67,    72,    73,    75,    76,
      80,    82,    86,    89,    90,    96,   102,   111,   119,   127,
     133,   137,   141,   144,   150,   158,   161,   164,   166,   168,
     170,   176,   184,   190,   192,   196,   198,   202,   204,   207,
     211,   212,   214,   216,   218,   220,   222,   224,   228,   232,
     234,   238,   242,   244,   247,   250,   252,   256,   260,   262,
     264,   266,   268,   273,   278,   285,   289,   294,   301,   305,
     307,   314,   319,   321,   323,   324,   328
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      56,     0,    -1,    56,    67,    -1,    56,    57,    -1,    56,
      62,    -1,    -1,    -1,     4,    51,    59,    58,    45,    60,
      46,    -1,    17,    51,    -1,    -1,    60,    61,    -1,    -1,
      67,    -1,    66,    51,    40,    -1,    11,    51,    45,    63,
      46,    -1,    64,    -1,    -1,    64,    65,    -1,    65,    -1,
      66,    51,    40,    -1,    18,    -1,    19,    -1,    20,    -1,
      21,    -1,     3,    51,    68,    71,    -1,     5,    43,    69,
      44,    -1,    -1,    70,    -1,    -1,    70,    41,    51,    -1,
      51,    -1,    45,    72,    46,    -1,    72,    73,    -1,    -1,
       8,    51,    28,    76,    40,    -1,    10,    51,    28,    76,
      40,    -1,    10,    51,    47,    76,    48,    28,    76,    40,
      -1,    10,    51,    42,    51,    28,    76,    40,    -1,    10,
      52,    42,    51,    28,    76,    40,    -1,    53,    43,    86,
      44,    40,    -1,     6,    76,    40,    -1,     7,    76,    40,
      -1,     7,    40,    -1,    51,    43,    86,    44,    40,    -1,
      51,    42,    51,    43,    86,    44,    40,    -1,    24,    40,
      -1,    25,    40,    -1,    71,    -1,    74,    -1,    75,    -1,
      13,    43,    76,    44,    71,    -1,    13,    43,    76,    44,
      71,    14,    71,    -1,    15,    43,    76,    44,    71,    -1,
      77,    -1,    77,    30,    78,    -1,    78,    -1,    78,    29,
      79,    -1,    79,    -1,    82,    80,    -1,    80,    81,    82,
      -1,    -1,    35,    -1,    36,    -1,    37,    -1,    38,    -1,
      26,    -1,    27,    -1,    82,    31,    83,    -1,    82,    32,
      83,    -1,    83,    -1,    83,    33,    84,    -1,    83,    34,
      84,    -1,    84,    -1,    32,    84,    -1,    39,    84,    -1,
      85,    -1,    43,    76,    44,    -1,    47,    86,    48,    -1,
      22,    -1,    23,    -1,    49,    -1,    50,    -1,    51,    47,
      76,    48,    -1,    51,    43,    86,    44,    -1,    51,    42,
      51,    43,    86,    44,    -1,    51,    42,    51,    -1,    52,
      43,    86,    44,    -1,    52,    42,    51,    43,    86,    44,
      -1,    52,    42,    51,    -1,    52,    -1,    53,    42,    51,
      43,    86,    44,    -1,    53,    43,    86,    44,    -1,    51,
      -1,    87,    -1,    -1,    87,    41,    76,    -1,    76,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    95,    95,    96,    97,    98,   104,   103,   121,   122,
     126,   127,   131,   132,   143,   148,   149,   153,   164,   168,
     175,   176,   177,   178,   182,   202,   203,   207,   208,   212,
     214,   220,   224,   225,   229,   232,   235,   238,   241,   244,
     247,   250,   253,   256,   259,   262,   264,   267,   269,   270,
     275,   277,   283,   289,   293,   294,   298,   299,   303,   311,
     312,   316,   317,   318,   319,   320,   321,   325,   326,   327,
     331,   332,   333,   337,   338,   339,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     358,   359,   360,   364,   365,   369,   370
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FLOW", "SYMPHONY", "TAKE", "EMIT",
  "RETURN", "NOTE", "FIXED", "STAGE", "ENSEMBLE", "PLAY", "BRANCH",
  "ELSEWISE", "REPEAT", "SCORE", "EXTENDS", "TYPE_INT", "TYPE_FLOAT",
  "TYPE_BOOL", "TYPE_STRING", "TRUE", "FALSE", "BREAK", "CONTINUE", "EQ",
  "NE", "ASSIGN", "AND", "OR", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE",
  "GT", "GE", "NOT", "SEMICOLON", "COMMA", "DOT", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "NUMBER", "STRING_LITERAL",
  "IDENTIFIER", "THIS", "SUPER", "UMINUS", "$accept", "program",
  "symphony_definition", "$@1", "extends_opt", "symphony_members",
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
     305,   306,   307,   308,   309
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    56,    56,    56,    56,    58,    57,    59,    59,
      60,    60,    61,    61,    62,    63,    63,    64,    64,    65,
      66,    66,    66,    66,    67,    68,    68,    69,    69,    70,
      70,    71,    72,    72,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      74,    74,    75,    76,    77,    77,    78,    78,    79,    80,
      80,    81,    81,    81,    81,    81,    81,    82,    82,    82,
      83,    83,    83,    84,    84,    84,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    86,    86,    87,    87
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     0,     7,     2,     0,
       2,     0,     1,     3,     5,     1,     0,     2,     1,     3,
       1,     1,     1,     1,     4,     4,     0,     1,     0,     3,
       1,     3,     2,     0,     5,     5,     8,     7,     7,     5,
       3,     3,     2,     5,     7,     2,     2,     1,     1,     1,
       5,     7,     5,     1,     3,     1,     3,     1,     2,     3,
       0,     1,     1,     1,     1,     1,     1,     3,     3,     1,
       3,     3,     1,     2,     2,     1,     3,     3,     1,     1,
       1,     1,     4,     4,     6,     3,     4,     6,     3,     1,
       6,     4,     1,     1,     0,     3,     1
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
       0,     0,     0,     0,     0,     0,     0,     0,    31,     0,
       0,    47,    32,    48,    49,     0,    19,    29,    78,    79,
       0,     0,     0,    94,    80,    81,    92,    89,     0,     0,
      53,    55,    57,    60,    69,    72,    75,    42,     0,     0,
       0,     0,     0,     0,    45,    46,     0,    94,    94,     7,
      10,     0,    12,    73,    74,     0,    96,     0,    93,     0,
      94,     0,     0,    94,     0,    94,    40,     0,     0,     0,
       0,    58,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,    77,     0,    85,
       0,     0,    88,     0,     0,     0,    54,    56,    67,    68,
      65,    66,    61,    62,    63,    64,     0,    70,    71,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    13,
      95,    94,    83,    82,    94,    86,    94,    91,    59,    34,
      35,     0,     0,     0,    50,    52,     0,    43,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    84,    87,    90,
      37,     0,    38,    51,    44,    36
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     6,    21,    15,    55,    90,     7,    26,    27,
      28,    29,     8,    13,    31,    32,    19,    33,    52,    53,
      54,    96,    70,    71,    72,   111,   146,    73,    74,    75,
      76,    97,    98
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -84
static const yytype_int16 yypact[] =
{
     -84,    42,   -84,   -48,   -43,   -36,   -84,   -84,   -84,     7,
       4,   -21,    -2,     2,     1,   -84,    45,    21,   -84,   -84,
     -84,    24,   -84,   -84,   -84,   -84,    36,    45,   -84,    35,
     -84,    46,    55,     3,   -84,   -84,   -84,    54,   -84,    56,
      86,    48,    60,   -12,    61,    62,    66,    70,   -84,    18,
      69,   -84,   -84,   -84,   -84,    11,   -84,   -84,   -84,   -84,
      86,    86,    86,    86,   -84,   -84,     8,    25,    32,    73,
      84,    87,   -84,    53,    59,   -84,   -84,   -84,    75,    89,
      -9,    77,    86,    86,   -84,   -84,    72,    86,    86,   -84,
     -84,    76,   -84,   -84,   -84,    80,   -84,    78,    79,    90,
      86,    86,    91,    86,    92,    86,   -84,    86,    86,    86,
      86,    -1,    86,    86,   -84,    86,    86,    94,    86,    95,
      96,   103,    85,   104,   105,   110,   -84,   -84,    86,    88,
     107,    82,   111,   109,   112,   113,    87,   -84,    59,    59,
     -84,   -84,   -84,   -84,   -84,   -84,    86,   -84,   -84,   116,
     118,   131,   114,   132,     2,     2,    86,   121,   123,   -84,
     -84,    86,   -84,   -84,    86,   -84,    86,   -84,    53,   -84,
     -84,    86,   136,    86,   151,   -84,   122,   -84,   -84,   124,
     125,   126,   127,    86,   133,     2,   134,   -84,   -84,   -84,
     -84,   135,   -84,   -84,   -84,   -84
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     144,   117,   128,   -84,   -84,   -84,   -33,   -84,   -84,   -84,
     -84,   -39,   -84,    71,    68,   -84,   -84,    31,    -7,   -54,
     -84,   -83,   -84
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      51,    69,    78,     9,   123,   124,    93,    94,    10,    40,
      41,    42,    12,    43,     3,    11,    44,   130,    45,   116,
     133,    14,   135,    95,    16,   140,   141,    46,    47,    22,
      23,    24,    25,   117,   142,   143,   144,   145,   118,    80,
      81,    17,     2,   120,   121,     3,     4,    18,    18,    48,
      99,   100,    20,     5,    49,   101,    50,    89,   147,   148,
      86,    87,   131,    22,    23,    24,    25,   102,   103,    34,
      58,    59,    30,   176,   104,   105,   149,   150,   179,   152,
      60,   180,    35,   181,   109,   110,    37,    61,    77,   160,
      38,    62,   112,   113,    56,    63,    39,    64,    65,    66,
      67,    68,   138,   139,    82,    83,    84,    57,    58,    59,
      85,    79,    88,   106,   107,   114,   108,   115,    60,   119,
     128,   174,   175,   122,   126,    61,   127,   125,   156,    62,
     163,   161,   182,    63,   184,    64,    65,    66,    67,    68,
     154,   129,   132,   134,   191,   151,   153,   155,   157,   158,
     159,   162,   193,   165,   164,   166,   169,   167,   170,   171,
     173,   177,   172,   178,   183,   185,   186,   190,   187,   188,
     189,    36,    91,   192,   194,   195,   137,   168,   136,     0,
       0,     0,     0,    92
};

static const yytype_int16 yycheck[] =
{
      33,    40,    41,    51,    87,    88,    60,    61,    51,     6,
       7,     8,     5,    10,     3,    51,    13,   100,    15,    28,
     103,    17,   105,    62,    45,    26,    27,    24,    25,    18,
      19,    20,    21,    42,    35,    36,    37,    38,    47,    51,
      52,    43,     0,    82,    83,     3,     4,    45,    45,    46,
      42,    43,    51,    11,    51,    47,    53,    46,   112,   113,
      42,    43,   101,    18,    19,    20,    21,    42,    43,    45,
      22,    23,    51,   156,    42,    43,   115,   116,   161,   118,
      32,   164,    46,   166,    31,    32,    51,    39,    40,   128,
      44,    43,    33,    34,    40,    47,    41,    49,    50,    51,
      52,    53,   109,   110,    43,    43,    40,    51,    22,    23,
      40,    51,    43,    40,    30,    40,    29,    28,    32,    42,
      41,   154,   155,    51,    44,    39,    48,    51,    43,    43,
      48,    43,   171,    47,   173,    49,    50,    51,    52,    53,
      44,    51,    51,    51,   183,    51,    51,    44,    44,    44,
      40,    44,   185,    44,    43,    43,    40,    44,    40,    28,
      28,    40,    48,    40,    28,    14,    44,    40,    44,    44,
      44,    27,    55,    40,    40,    40,   108,   146,   107,    -1,
      -1,    -1,    -1,    55
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    56,     0,     3,     4,    11,    57,    62,    67,    51,
      51,    51,     5,    68,    17,    59,    45,    43,    45,    71,
      51,    58,    18,    19,    20,    21,    63,    64,    65,    66,
      51,    69,    70,    72,    45,    46,    65,    51,    44,    41,
       6,     7,     8,    10,    13,    15,    24,    25,    46,    51,
      53,    71,    73,    74,    75,    60,    40,    51,    22,    23,
      32,    39,    43,    47,    49,    50,    51,    52,    53,    76,
      77,    78,    79,    82,    83,    84,    85,    40,    76,    51,
      51,    52,    43,    43,    40,    40,    42,    43,    43,    46,
      61,    66,    67,    84,    84,    76,    76,    86,    87,    42,
      43,    47,    42,    43,    42,    43,    40,    30,    29,    31,
      32,    80,    33,    34,    40,    28,    28,    42,    47,    42,
      76,    76,    51,    86,    86,    51,    44,    48,    41,    51,
      86,    76,    51,    86,    51,    86,    78,    79,    83,    83,
      26,    27,    35,    36,    37,    38,    81,    84,    84,    76,
      76,    51,    76,    51,    44,    44,    43,    44,    44,    40,
      76,    43,    44,    48,    43,    44,    43,    44,    82,    40,
      40,    28,    48,    28,    71,    71,    86,    40,    40,    86,
      86,    86,    76,    28,    76,    14,    44,    44,    44,    44,
      40,    76,    40,    71,    40,    40
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
#line 104 "orchestra.y"
    {
      /* Enter class/ensemble scope for name qualification. */
      g_current_class = (yyvsp[(2) - (3)].sval);
      g_current_symphony_fields = NULL;
      /* stash parent name in a simple way: store it in g_current_symphony_fields later via register */
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 111 "orchestra.y"
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
#line 121 "orchestra.y"
    { (yyval.sval) = (yyvsp[(2) - (2)].sval); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 122 "orchestra.y"
    { (yyval.sval) = NULL; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 133 "orchestra.y"
    {
        g_current_symphony_fields = fieldlist_append(g_current_symphony_fields, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval));
      ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 144 "orchestra.y"
    { register_ensemble((yyvsp[(2) - (5)].sval), NULL, (yyvsp[(4) - (5)].field_list)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 149 "orchestra.y"
    { (yyval.field_list) = NULL; ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 154 "orchestra.y"
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
#line 164 "orchestra.y"
    { (yyval.field_list) = (yyvsp[(1) - (1)].field_list); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 169 "orchestra.y"
    {
        (yyval.field_list) = fieldlist_append(NULL, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval));
      ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 175 "orchestra.y"
    { (yyval.sval) = arena_strdup("int"); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 176 "orchestra.y"
    { (yyval.sval) = arena_strdup("float"); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 177 "orchestra.y"
    { (yyval.sval) = arena_strdup("bool"); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 178 "orchestra.y"
    { (yyval.sval) = arena_strdup("string"); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 183 "orchestra.y"
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
#line 202 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(3) - (4)].name_list); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 203 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 207 "orchestra.y"
    { (yyval.name_list) = (yyvsp[(1) - (1)].name_list); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 208 "orchestra.y"
    { (yyval.name_list) = NULL; ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 213 "orchestra.y"
    { (yyval.name_list) = namelist_append((yyvsp[(1) - (3)].name_list), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 215 "orchestra.y"
    { (yyval.name_list) = namelist_append(NULL, (yyvsp[(1) - (1)].sval)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 220 "orchestra.y"
    { (yyval.stmt) = make_block((yyvsp[(2) - (3)].stmt_list)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 224 "orchestra.y"
    { (yyval.stmt_list) = stmtlist_append((yyvsp[(1) - (2)].stmt_list), (yyvsp[(2) - (2)].stmt)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 225 "orchestra.y"
    { (yyval.stmt_list) = NULL; ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 230 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_NOTE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 233 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_STAGE, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].expr)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 236 "orchestra.y"
    { (yyval.stmt) = make_index_stage((yyvsp[(2) - (8)].sval), (yyvsp[(4) - (8)].expr), (yyvsp[(7) - (8)].expr)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 239 "orchestra.y"
    { (yyval.stmt) = make_field_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].sval), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 242 "orchestra.y"
    { (yyval.stmt) = make_field_stage((yyvsp[(2) - (7)].sval), (yyvsp[(4) - (7)].sval), (yyvsp[(6) - (7)].expr)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 245 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_superctor((yyvsp[(3) - (5)].expr_list))); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 248 "orchestra.y"
    { (yyval.stmt) = make_emit((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 251 "orchestra.y"
    { (yyval.stmt) = make_return((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 254 "orchestra.y"
    { (yyval.stmt) = make_return(NULL); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 257 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_call((yyvsp[(1) - (5)].sval), (yyvsp[(3) - (5)].expr_list))); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 260 "orchestra.y"
    { (yyval.stmt) = make_expr_stmt(make_dotcall((yyvsp[(1) - (7)].sval), (yyvsp[(3) - (7)].sval), (yyvsp[(5) - (7)].expr_list))); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 263 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_BREAK, NULL, NULL); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 265 "orchestra.y"
    { (yyval.stmt) = make_assign(STMT_CONTINUE, NULL, NULL); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 276 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt), NULL); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 278 "orchestra.y"
    { (yyval.stmt) = make_branch((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 284 "orchestra.y"
    { (yyval.stmt) = make_repeat((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt)); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 289 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 293 "orchestra.y"
    { (yyval.expr) = make_bin(OP_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 294 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 298 "orchestra.y"
    { (yyval.expr) = make_bin(OP_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 299 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 304 "orchestra.y"
    {
        if ((yyvsp[(2) - (2)].chain_parts)) (yyval.expr) = make_chain((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].chain_parts));
        else (yyval.expr) = (yyvsp[(1) - (2)].expr);
      ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 311 "orchestra.y"
    { (yyval.chain_parts) = chainpart_append((yyvsp[(1) - (3)].chain_parts), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 312 "orchestra.y"
    { (yyval.chain_parts) = NULL; ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 316 "orchestra.y"
    { (yyval.ival) = OP_LT; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 317 "orchestra.y"
    { (yyval.ival) = OP_LE; ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 318 "orchestra.y"
    { (yyval.ival) = OP_GT; ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 319 "orchestra.y"
    { (yyval.ival) = OP_GE; ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 320 "orchestra.y"
    { (yyval.ival) = OP_EQ; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 321 "orchestra.y"
    { (yyval.ival) = OP_NE; ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 325 "orchestra.y"
    { (yyval.expr) = make_bin(OP_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 326 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 327 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 331 "orchestra.y"
    { (yyval.expr) = make_bin(OP_MUL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 332 "orchestra.y"
    { (yyval.expr) = make_bin(OP_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 333 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 337 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NEG, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 338 "orchestra.y"
    { (yyval.expr) = make_unary(OP_NOT, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 339 "orchestra.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 343 "orchestra.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 344 "orchestra.y"
    { (yyval.expr) = make_arraylit((yyvsp[(2) - (3)].expr_list)); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 345 "orchestra.y"
    { (yyval.expr) = make_lit_bool(1); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 346 "orchestra.y"
    { (yyval.expr) = make_lit_bool(0); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 347 "orchestra.y"
    { (yyval.expr) = make_lit_num((yyvsp[(1) - (1)].numlit).num, (yyvsp[(1) - (1)].numlit).is_float); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 348 "orchestra.y"
    { (yyval.expr) = make_lit_string((yyvsp[(1) - (1)].sval)); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 349 "orchestra.y"
    { (yyval.expr) = make_index(make_var((yyvsp[(1) - (4)].sval)), (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 350 "orchestra.y"
    { (yyval.expr) = make_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 351 "orchestra.y"
    { (yyval.expr) = make_dotcall((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 352 "orchestra.y"
    { (yyval.expr) = make_field((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 353 "orchestra.y"
    { (yyval.expr) = make_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 354 "orchestra.y"
    { (yyval.expr) = make_dotcall((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 355 "orchestra.y"
    { (yyval.expr) = make_field((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 356 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 358 "orchestra.y"
    { (yyval.expr) = make_supercall((yyvsp[(3) - (6)].sval), (yyvsp[(5) - (6)].expr_list)); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 359 "orchestra.y"
    { (yyval.expr) = make_superctor((yyvsp[(3) - (4)].expr_list)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 360 "orchestra.y"
    { (yyval.expr) = make_var((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 364 "orchestra.y"
    { (yyval.expr_list) = (yyvsp[(1) - (1)].expr_list); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 365 "orchestra.y"
    { (yyval.expr_list) = NULL; ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 369 "orchestra.y"
    { (yyval.expr_list) = exprlist_append((yyvsp[(1) - (3)].expr_list), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 370 "orchestra.y"
    { (yyval.expr_list) = exprlist_append(NULL, (yyvsp[(1) - (1)].expr)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2234 "orchestra.tab.c"
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
#line 373 "orchestra.y"


int main(int argc, char *argv[]) {
  const char *backend = "ast";

  /* minimal CLI: orchestra.exe input.txt output.txt [--backend=ast|vm] */
  if (argc >= 4) {
    if (strcmp(argv[3], "--backend=vm") == 0) backend = "vm";
    else if (strcmp(argv[3], "--backend=ast") == 0) backend = "ast";
    else if (strcmp(argv[3], "--backend") == 0 && argc >= 5) backend = argv[4];
  }

  if (argc < 3) {
    printf("Usage: %s input.txt output.txt [--backend=ast|vm]\n", argv[0]);
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
      if (strcmp(backend, "vm") == 0) {
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
    fprintf(yyout ? yyout : stdout, "Syntax Error: %s\n", s);
    return 0;
}
