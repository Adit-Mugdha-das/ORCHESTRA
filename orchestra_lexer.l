%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
"flow"      { fprintf(yyout, "KEYWORD_FLOW\n"); }
"take"      { fprintf(yyout, "KEYWORD_TAKE\n"); }
"emit"      { fprintf(yyout, "KEYWORD_EMIT\n"); }
"note"      { fprintf(yyout, "KEYWORD_NOTE\n"); }
"fixed"     { fprintf(yyout, "KEYWORD_FIXED\n"); }
"stage"     { fprintf(yyout, "KEYWORD_STAGE\n"); }
"ensemble"  { fprintf(yyout, "KEYWORD_ENSEMBLE\n"); }
"play"      { fprintf(yyout, "KEYWORD_PLAY\n"); }
"branch"    { fprintf(yyout, "KEYWORD_BRANCH\n"); }
"elsewise"  { fprintf(yyout, "KEYWORD_ELSEWISE\n"); }
"repeat"    { fprintf(yyout, "KEYWORD_REPEAT\n"); }
"score"     { fprintf(yyout, "KEYWORD_SCORE\n"); }

"int"       { fprintf(yyout, "TYPE_INT\n"); }
"float"     { fprintf(yyout, "TYPE_FLOAT\n"); }
"bool"      { fprintf(yyout, "TYPE_BOOL\n"); }
"string"    { fprintf(yyout, "TYPE_STRING\n"); }

[0-9]+(\.[0-9]+)?   { fprintf(yyout, "NUMBER\n"); }

[a-zA-Z][a-zA-Z0-9_]*  { fprintf(yyout, "IDENTIFIER\n"); }

"=="|"+"|"-"|"*"|"/"|"&&"|"||"|"="|"<"|">" { fprintf(yyout, "OPERATOR\n"); }

\"([^\\\"]|\\.)*\"    { fprintf(yyout, "STRING_LITERAL\n"); }

[ \t\n]+    ;

.           { fprintf(yyout, "INVALID_TOKEN: %s\n", yytext); }
%%

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

    yylex();

    fclose(input);
    fclose(output);
    return 0;
}
