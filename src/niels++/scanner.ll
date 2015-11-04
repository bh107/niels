%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "driver.hh"
# include "parser.hh"

# undef yywrap
# define yywrap() 1

static nls::location loc; // Location of the current token.
%}
%option noyywrap nounput batch debug noinput
id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t]

%{
    // Code run each time a pattern is matched.
    # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
    // Code run each time yylex is called.
    loc.step ();
%}

{blank}+   loc.step ();
[\n]+      loc.lines (yyleng); loc.step ();
"-"      return nls::Parser::make_MINUS(loc);
"+"      return nls::Parser::make_PLUS(loc);
"*"      return nls::Parser::make_STAR(loc);
"/"      return nls::Parser::make_SLASH(loc);
"("      return nls::Parser::make_LPAREN(loc);
")"      return nls::Parser::make_RPAREN(loc);
":="     return nls::Parser::make_ASSIGN(loc);

{int}      {
    errno = 0;
    long n = strtol (yytext, NULL, 10);
    if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
        driver.error (loc, "integer is out of range");
    }
    return nls::Parser::make_NUMBER(n, loc);
}

{id}       return nls::Parser::make_IDENTIFIER(yytext, loc);
.          driver.error (loc, "invalid character");
<<EOF>>    return nls::Parser::make_END(loc);
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

