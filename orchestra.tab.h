
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
  struct ExprList;
  struct NameList;
  struct ChainPart;



/* Line 1676 of yacc.c  */
#line 51 "orchestra.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FLOW = 258,
     TAKE = 259,
     EMIT = 260,
     RETURN = 261,
     NOTE = 262,
     FIXED = 263,
     STAGE = 264,
     ENSEMBLE = 265,
     PLAY = 266,
     BRANCH = 267,
     ELSEWISE = 268,
     REPEAT = 269,
     SCORE = 270,
     TYPE_INT = 271,
     TYPE_FLOAT = 272,
     TYPE_BOOL = 273,
     TYPE_STRING = 274,
     TRUE = 275,
     FALSE = 276,
     BREAK = 277,
     CONTINUE = 278,
     EQ = 279,
     NE = 280,
     ASSIGN = 281,
     AND = 282,
     OR = 283,
     PLUS = 284,
     MINUS = 285,
     MUL = 286,
     DIV = 287,
     LT = 288,
     LE = 289,
     GT = 290,
     GE = 291,
     NOT = 292,
     SEMICOLON = 293,
     COMMA = 294,
     LPAREN = 295,
     RPAREN = 296,
     LBRACE = 297,
     RBRACE = 298,
     NUMBER = 299,
     STRING_LITERAL = 300,
     IDENTIFIER = 301,
     UMINUS = 302
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 28 "orchestra.y"

  struct { double num; int is_float; } numlit;
  char *sval;
  int ival;
  struct Expr *expr;
  struct Stmt *stmt;
  struct StmtList *stmt_list;
  struct ExprList *expr_list;
  struct NameList *name_list;
  struct ChainPart *chain_parts;



/* Line 1676 of yacc.c  */
#line 129 "orchestra.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


