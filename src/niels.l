niels.l
%{
#include "ast.h"
#include "niels.tab.hpp"
#include <iostream>
#include <string>
using namespace std;
stringstream _comment;
#define YY_DECL extern "C" int yylex()

%}
%option noyywrap
%x IN_COMMENT
%%
[ \t]   ; // Ignore whitespace

<INITIAL>{
    "/*"    {
        BEGIN(IN_COMMENT);
    }
}
<IN_COMMENT>{
    "*/"    {
        yylval.str = strdup(_comment.str().c_str());
        _comment.str("");   // Reset the stream
        _comment.clear();

        BEGIN(INITIAL);
        return COMMENT;
    }
    [^*\n]+ { _comment << yytext; }
    "*"     { _comment << yytext; }
    \n      {
        _comment << yytext;
        yylineno++;
    }
}

"@"     { return AT; }
"?"     { return QUERY; }

"="     { return ALIAS; }
"<-"    { return ASSIGN; }

"!"     { return INVERT; }
"~"     { return NEGATE; }

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

"as"    { return AS; }

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

"import" { return IMPORT; }

"function" { return FUNCTION; }
"record" { return RECORD; }

"return" { return RETURN; }
"while" { return WHILE; }

"when"      { return WHEN; }
"is"        { return IS; }
"otherwise" { return OTHERWISE; }

"true" {
    yylval.boolean = true;
    return BOOL;
}
"false" {
    yylval.boolean = false;
    return BOOL;
}
"bool" {
    yylval.boolean = true;
    return BOOL;
}

[0-9]+\.[0-9]+  {
    yylval.r64 = atof(yytext);
    return REAL64;
}
"real(32)" {
    yylval.r32 = atof(yytext);
    return REAL32;
}
"real(64)" {
    yylval.r64 = atof(yytext);
    return REAL64;
}
"real" {
    yylval.r64 = atof(yytext);
    return REAL64;
}

[0-9]+          {
    yylval.i64 = atol(yytext);
    return INT64;
}
"int(32)" {
    yylval.i32 = atoi(yytext);
    return INT32;
}
"int(64)" {
    yylval.i64 = atol(yytext);
    return INT64;
}
"int" {
    yylval.i64 = atol(yytext);
    return INT64;
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

