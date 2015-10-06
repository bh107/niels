niels.l
%{
#include "ast.h"
#include "niels.tab.hpp"
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()

%}
%option noyywrap
%%
[ \t]   ; // Ignore whitespace
"!"     { return INVERT; }
"~"     { return NEGATE; }
"?"     { return QUERY; }

"="     { return ALIAS; }
"<-"    { return ASSIGN; }

"+"     { return ADD; }
"-"     { return SUB; }

"*"     { return MUL; }
"/"     { return DIV; }
"%"     { return MOD; }
"^"     { return POW; }

"<"     { return LTHAN; }
">"     { return GTHAN; }
"=="    { return EQUAL; }
"<="    { return LTHAN_EQUAL; }
">="    { return GTHAN_EQUAL; }

"or"    { return OR; }
"xor"   { return XOR; }
"and"   { return AND; }

"as"    { return SHAPER; }

"\n"    { return NL; }
","     { return COMMA;  }
"("     { return LPAREN; }
")"     { return RPAREN; }
"["     { return LBRACK; }
"]"     { return RBRACK; }
"{"     { return LBRACE; }
"}"     { return RBRACE; }
":"     { return COLON; }
";"     { return SEMICOLON; }

true {
    yylval.boolean = true;
    return BOOL;
}
false {
    yylval.boolean = false;
    return BOOL;
}
[0-9]+\.[0-9]+  {
    yylval.r64 = atof(yytext);
    return REAL;
}
[0-9]+          {
    yylval.i64 = atoi(yytext);
    return INT;
}
\".+\" {
    yylval.str = strdup(yytext);
    return STRING;
}
'[a-zA-Z0-9]+' {
    yylval.str = strdup(yytext);
    return STRING;
}
[a-zA-Z]+[a-zA-Z0-9]? {
    yylval.str = strdup(yytext);
    return IDENT;
}

%%

/*
YYSTYPE yylval;

int main(void)
{
    int token;
    while ((token = yylex()) != 0)
        printf("Token: %d (%s)\n", token, yytext);
    return 0;
}
*/

