%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include "driver.hh"
#include "parser.hh"

#undef yywrap
#define yywrap() 1

static nls::location loc; // Location of the current token.
%}
%option noyywrap nounput batch debug noinput

%{
    #define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
    loc.step();
%}

[ \t]+  {
    loc.step();
}
[\n]+   {
    loc.lines(yyleng);
    loc.step();
}
"<-" {
    return nls::Parser::make_ASSIGN(loc);
}
"=" {
    return nls::Parser::make_ALIAS(loc);
}
"(" {
    return nls::Parser::make_LPAREN(loc);
}
")" {
    return nls::Parser::make_RPAREN(loc);
}
"+"     {
    return nls::Parser::make_ADD(loc);
}
"int(32)" {
    return nls::Parser::make_INT32(atoi(yytext), loc);
}
"int(64)" {
    return nls::Parser::make_INT64(atoi(yytext), loc);
}
"int" {
    return nls::Parser::make_INT64(atoi(yytext), loc);
}
[0-9]+  {
    return nls::Parser::make_INT64(atoi(yytext), loc);
}
[a-zA-Z]+[a-zA-Z0-9]? {
    return nls::Parser::make_IDENT(yytext, loc);
}
. {
    driver.error (loc, "invalid character");
}
<<EOF>> {
    return nls::Parser::make_END(loc);
}
%%

void nls::Driver::scan_begin ()
{
    yy_flex_debug = trace_scanning;
    if (file.empty () || file == "-") {
        yyin = stdin;
    } else if (!(yyin = fopen (file.c_str (), "r"))) {
      error ("cannot open " + file + ": " + strerror(errno));
      exit (EXIT_FAILURE);
    }
}

void nls::Driver::scan_end ()
{
    fclose (yyin);
}

