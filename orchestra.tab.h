
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
     ASSIGN = 277,
     AND = 278,
     OR = 279,
     PLUS = 280,
     MINUS = 281,
     MUL = 282,
     DIV = 283,
     LT = 284,
     LE = 285,
     GT = 286,
     NOT = 287,
     SEMICOLON = 288,
     COMMA = 289,
     LPAREN = 290,
     RPAREN = 291,
     LBRACE = 292,
     RBRACE = 293,
     NUMBER = 294,
     STRING_LITERAL = 295,
     IDENTIFIER = 296,
     UMINUS = 297
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
#line 117 "orchestra.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


