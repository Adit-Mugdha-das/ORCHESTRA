
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 19 "orchestra.y"

  struct Expr;
  struct Stmt;
  struct StmtList;



/* Line 1676 of yacc.c  */
#line 48 "orchestra.tab.h"

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
     TRUE = 274,
     FALSE = 275,
     EQ = 276,
     NE = 277,
     ASSIGN = 278,
     AND = 279,
     OR = 280,
     PLUS = 281,
     MINUS = 282,
     MUL = 283,
     DIV = 284,
     LT = 285,
     LE = 286,
     GT = 287,
     GE = 288,
     NOT = 289,
     SEMICOLON = 290,
     COMMA = 291,
     LPAREN = 292,
     RPAREN = 293,
     LBRACE = 294,
     RBRACE = 295,
     NUMBER = 296,
     STRING_LITERAL = 297,
     IDENTIFIER = 298,
     UMINUS = 299
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 25 "orchestra.y"

  struct { double num; int is_float; } numlit;
  char *sval;
  struct Expr *expr;
  struct Stmt *stmt;
  struct StmtList *stmt_list;



/* Line 1676 of yacc.c  */
#line 119 "orchestra.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


