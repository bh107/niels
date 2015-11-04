%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {Parser}

%define api.token.constructor
%define api.namespace {nls}
%define api.value.type variant
%define parse.assert
%code requires
{
# include <string>
namespace nls {
    class Driver;
}
}
// The parsing context.
%param { Driver& driver }
%locations
%initial-action
{
    // Initialize the initial location.
    @$.begin.filename = @$.end.filename = &driver.file;
};
%define parse.trace
%define parse.error verbose
%code
{
# include "driver.hh"
}
%define api.token.prefix {TOK_}
%token
    END  0  "end of file"
    ASSIGN  ":="
    MINUS   "-"
    PLUS    "+"
    STAR    "*"
    SLASH   "/"
    LPAREN  "("
    RPAREN  ")"
;
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%type  <int> exp
%printer { yyoutput << $$; } <*>;
%%
%start unit;
unit: assignments exp  { driver.result = $2; };

assignments:
  %empty                 {}
| assignments assignment {};

assignment:
    IDENTIFIER ":=" exp {
        driver.variables[$1] = $3;
    }
;

%left "+" "-";
%left "*" "/";
exp:
  exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp   { $$ = $1 / $3; }
| "(" exp ")"   { std::swap ($$, $2); }
| IDENTIFIER  { $$ = driver.variables[$1]; }
| NUMBER      { std::swap ($$, $1); };
%%
void nls::Parser::error (const location_type& l, const std::string& m)
{
  driver.error (l, m);
}
