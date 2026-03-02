%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orchestra.tab.h"
%}

%%

"flow"      return FLOW;
"take"      return TAKE;
"emit"      return EMIT;
"note"      return NOTE;
"fixed"     return FIXED;
"stage"     return STAGE;
"ensemble"  return ENSEMBLE;
"play"      return PLAY;
"branch"    return BRANCH;
"elsewise"  return ELSEWISE;
"repeat"    return REPEAT;
"score"     return SCORE;

"int"       return TYPE_INT;
"float"     return TYPE_FLOAT;
"bool"      return TYPE_BOOL;
"string"    return TYPE_STRING;

"=="        return EQ;
"="         return ASSIGN;
"&&"        return AND;
"||"        return OR;
"+"         return PLUS;
"-"         return MINUS;
"*"         return MUL;
"//"[^\n]*  ;
"/"         return DIV;
"<"         return LT;
">"         return GT;

";"         return SEMICOLON;
","         return COMMA;
"("         return LPAREN;
")"         return RPAREN;
"{"         return LBRACE;
"}"         return RBRACE;

[0-9]+\.[0-9]+ {
    strcpy(yylval.expr.type, "float");
    return NUMBER;
}

[0-9]+ {
    strcpy(yylval.expr.type, "int");
    return NUMBER;
}

\"([^\\\"]|\\.)*\" {
    strcpy(yylval.expr.type, "string");
    return STRING_LITERAL;
}

[a-zA-Z][a-zA-Z0-9_]* {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

[ \t\r\n]+   ;

. {
    printf("Invalid token: %s\n", yytext);
}

%%