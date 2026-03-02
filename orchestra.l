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

"true"      return TRUE;
"false"     return FALSE;

"=="        return EQ;
"="         return ASSIGN;
"&&"        return AND;
"||"        return OR;
"+"         return PLUS;
"-"         return MINUS;
"*"         return MUL;
"!"         return NOT;
"//"[^\n]*  ;
"/"         return DIV;
"<="        return LE;
"<"         return LT;
">"         return GT;

";"         return SEMICOLON;
","         return COMMA;
"("         return LPAREN;
")"         return RPAREN;
"{"         return LBRACE;
"}"         return RBRACE;

[0-9]+\.[0-9]+ {
    yylval.numlit.num = atof(yytext);
    yylval.numlit.is_float = 1;
    return NUMBER;
}

[0-9]+ {
    yylval.numlit.num = atof(yytext);
    yylval.numlit.is_float = 0;
    return NUMBER;
}

\"([^\\\"]|\\.)*\" {
    /* store string literal without surrounding quotes */
    int n = yyleng;
    char *s = (char*)malloc((size_t)n);
    if (!s) exit(1);
    if (n >= 2) {
        memcpy(s, yytext + 1, (size_t)(n - 2));
        s[n - 2] = '\0';
    } else {
        s[0] = '\0';
    }
    yylval.sval = s;
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