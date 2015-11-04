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
%option yylineno
%x IN_COMMENT
%%

<INITIAL>{
    "/*"    {
        BEGIN(IN_COMMENT);
    }
}
<IN_COMMENT>{
    "*/"    {
        yylval.node = new nir::Comment(_comment.str().c_str());
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

[ \t]   ; // Ignore whitespace
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
    yylval.node = new nir::Bool(true);
    return BOOL;
}
"false" {
    yylval.node = new nir::Bool(false);
    return BOOL;
}
"bool" {
    yylval.node = new nir::Bool();
    return BOOL;
}

[0-9]+\.[0-9]+  {
    yylval.node = new nir::Real64(atof(yytext));
    return REAL64;
}
"real(32)" {
    yylval.node = new nir::Real32();
    return REAL32;
}
"real(64)" {
    yylval.node = new nir::Real64();
    return REAL64;
}
"real" {
    yylval.node = new nir::Real64();
    return REAL64;
}

[0-9]+          {
    yylval.node = new nir::Int64(atol(yytext));
    return INT64;
}
"int(32)" {
    yylval.node = new nir::Int32();
    return INT32;
}
"int(64)" {
    yylval.node = new nir::Int64();
    return INT64;
}
"int" {
    yylval.node = new nir::Int64();
    return INT64;
}

\".+\" {
    yylval.node = new nir::Str(yytext);
    return STRING;
}
'[a-zA-Z0-9]+' {
    yylval.node = new nir::Str(yytext);
    return STRING;
}

[a-zA-Z]+[a-zA-Z0-9]? {
    yylval.node = new nir::Ident(yytext);
    return IDENT;
}

%%

