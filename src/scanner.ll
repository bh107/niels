scanner.ll
%{
#include <iostream>
#include <string>
#include <nls/nls.hh>
#include <parser.hh>

using namespace std;
stringstream _comment;
#define YY_DECL extern "C++" int yylex(nls::Driver& env)

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
        yylval.node = new nls::ast::Comment(_comment.str().c_str());
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
\/\/.*  ; // Ignore single-line comments
"@"     { return AT; }
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
"!="    { return NOT_EQUAL; }
"<="    { return LTHAN_EQUAL; }
">="    { return GTHAN_EQUAL; }

"and"   { return AND; }
"or"    { return OR; }
"xor"   { return XOR; }
"not"   { return NOT; }

"bw(and)"   { return BW_AND; }
"bw(or)"    { return BW_OR; }
"bw(xor)"   { return BW_XOR; }
"bw(not)"   { return BW_NOT; }

"lshift"    { return BW_LSHIFT; }
"rshift"    { return BW_RSHIFT; }

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
".."    { return DOTDOT; }
"."     { return DOT; }

"import" { return IMPORT; }

"function" { return FUNCTION; }
"record" { return RECORD; }
"new" { return NEW; }

"return" { return RETURN; }
"while" { return WHILE; }

"when"      { return WHEN; }
"is"        { return IS; }
"otherwise" { return OTHERWISE; }

"true" {
    yylval.node = new nls::ast::Bul("true");
    return BOOL;
}
"false" {
    yylval.node = new nls::ast::Bul("false");
    return BOOL;
}
"bool" {
    yylval.node = new nls::ast::Bul("false");
    return BOOL;
}

[0-9]+\.[0-9]+  {
    yylval.node = new nls::ast::R64(yytext);
    return REAL64;
}
"r32" {
    yylval.node = new nls::ast::R32("0");
    return REAL32;
}
"r64" {
    yylval.node = new nls::ast::R64("0");
    return REAL64;
}
"real" {
    yylval.node = new nls::ast::R64("0");
    return REAL64;
}

[0-9]+          {
    yylval.node = new nls::ast::I64(yytext);
    return INT64;
}
"i32" {
    yylval.node = new nls::ast::I32("0");
    return INT32;
}
"i64" {
    yylval.node = new nls::ast::I64("0");
    return INT64;
}
"int" {
    yylval.node = new nls::ast::I64("0");
    return INT64;
}

\"[^\"]*\" {
    yylval.node = new nls::ast::Str(yytext);
    return STRING;
}

[a-zA-Z]+[a-zA-Z0-9]? {
    yylval.node = new nls::ast::Ident(yytext);
    return IDENT;
}

. {
    cout << "Ignoring character(" << yytext << ")" << endl;
}
<<EOF>> {
    yyterminate();
}

%%

void nls::Driver::scanBegin(void)
{
    yy_flex_debug = _trace_scanning;
    if (_filename.empty () || _filename == "-") {
        yyin = stdin;
    } else if (!(yyin = fopen (_filename.c_str (), "r"))) {
        error ("cannot open " + _filename + ": " + strerror(errno));
        exit (EXIT_FAILURE);
    }
}

void nls::Driver::scanEnd(void)
{
    fclose(yyin);
}

