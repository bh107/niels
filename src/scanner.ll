scanner.l
%{
#include <iostream>
#include <string>
#include "ast.hh"
#include "parser.hh"

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
        yylval.node = new nls::Comment(_comment.str().c_str());
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
"!="    { return NOT_EQUAL; }
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
    yylval.node = new nls::Bool(true);
    return BOOL;
}
"false" {
    yylval.node = new nls::Bool(false);
    return BOOL;
}
"bool" {
    yylval.node = new nls::Bool();
    return BOOL;
}

[0-9]+\.[0-9]+  {
    yylval.node = new nls::Real64(atof(yytext));
    return REAL64;
}
"r32" {
    yylval.node = new nls::Real32();
    return REAL32;
}
"r64" {
    yylval.node = new nls::Real64();
    return REAL64;
}
"real" {
    yylval.node = new nls::Real64();
    return REAL64;
}

[0-9]+          {
    yylval.node = new nls::Int64(atol(yytext));
    return INT64;
}
"i32" {
    yylval.node = new nls::Int32();
    return INT32;
}
"i64" {
    yylval.node = new nls::Int64();
    return INT64;
}
"int" {
    yylval.node = new nls::Int64();
    return INT64;
}

\".+\" {
    yylval.node = new nls::Str(yytext);
    return STRING;
}
'[a-zA-Z0-9]+' {
    yylval.node = new nls::Str(yytext);
    return STRING;
}

[a-zA-Z]+[a-zA-Z0-9]? {
    yylval.node = new nls::Ident(yytext);
    return IDENT;
}

%%

