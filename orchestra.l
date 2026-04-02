%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orchestra.tab.h"
#include "interpreter.h"

extern FILE *yyout;

static void lex_error_at(int line, const char *msg, const char *near_text) {
    FILE *out = yyout ? yyout : stdout;
    fprintf(out, "Lexical Error: %s at line %d", msg ? msg : "error", line);
    if (near_text && near_text[0]) fprintf(out, " near '%s'", near_text);
    fprintf(out, "\n");
    fflush(out);

    if (out != stderr) {
        fprintf(stderr, "Lexical Error: %s at line %d", msg ? msg : "error", line);
        if (near_text && near_text[0]) fprintf(stderr, " near '%s'", near_text);
        fprintf(stderr, "\n");
        fflush(stderr);
    }
}
%}

%%

"flow"      return FLOW;
"symphony"  return SYMPHONY;
"extends"   return EXTENDS;
"take"      return TAKE;
"emit"      return EMIT;
"return"    return RETURN;
"note"      return NOTE;
"fixed"     return FIXED;
"stage"     return STAGE;
"ensemble"  return ENSEMBLE;
"play"      return PLAY;
"branch"    return BRANCH;
"elsewise"  return ELSEWISE;
"repeat"    return REPEAT;
"break"     return BREAK;
"continue"  return CONTINUE;
"score"     return SCORE;

"int"       return TYPE_INT;
"float"     return TYPE_FLOAT;
"bool"      return TYPE_BOOL;
"string"    return TYPE_STRING;

"true"      return TRUE;
"false"     return FALSE;

"this"      { yylval.sval = arena_strdup("this"); return THIS; }
"super"     { yylval.sval = arena_strdup("super"); return SUPER; }

"=="        return EQ;
"!="        return NE;
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
">="        return GE;
">"         return GT;

";"         return SEMICOLON;
","         return COMMA;
"."         return DOT;
"("         return LPAREN;
")"         return RPAREN;
"["         return LBRACKET;
"]"         return RBRACKET;
"{"         return LBRACE;
"}"         return RBRACE;

[0-9]+\.[0-9]+\.[0-9]+[0-9.]* {
    lex_error_at(yylineno, "invalid number format", yytext);
    exit(1);
}

[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? {
    yylval.numlit.num = atof(yytext);
    yylval.numlit.is_float = 1;
    return NUMBER;
}

[0-9]+[eE][+-]?[0-9]+ {
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
    yylval.sval = arena_strdup(yytext);
    return IDENTIFIER;
}

[ \t\r\n]+   ;

. {
    lex_error_at(yylineno, "unexpected character", yytext);
    exit(1);
}

%%