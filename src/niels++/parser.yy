%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {Parser}
%define api.namespace {nls}
%define api.value.type variant
%define api.token.constructor
%define parse.assert
%code requires
{
#include <string>
#include <ast.hh>
namespace nls {
    class Driver;
}
}
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
    ASSIGN  "<-"
    AS      "as"
    AND     "and"
    OR      "or"
    XOR     "xor"
    LTHAN   "<"
    GTHAN   ">"
    EQUAL   "=="
    LTHAN_EQUAL "<="
    GTHAN_EQUAL ">="
    INVERT  "!"
    NEGATE  "~"
    ALIAS   "="
    SUB     "-"
    ADD     "+"
    MUL     "*"
    DIV     "/"
    MOD     "%"
    QUERY   "?"
    LPAREN  "("
    RPAREN  ")"
    LBRACE  "{"
    RBRACE  "}"
    LBRACK  "["
    RBRACK  "]"
;
%token <int>    INT32   "int(32)"
%token <long>   INT64   "int(64)"
%token <float>  REAL32  "real(32)"
%token <double> REAL64  "real(64)"
%token <std::string> IDENT  "identifier"
%type  <Node> exp
%printer {
    yyoutput << "BLA";
} <*>;
%%
%start unit;
unit: exp  {
        driver.ast = &$1;
    }
;

exp:
  exp "+" exp   { $$ = new nls::Add($1, $3); }
| LPAREN exp RPAREN { $$ = $2; }
| INT32         { $$ = new nls::Int32($1); }
| INT64         { $$ = new nls::Int64($1); }
| REAL32        { $$ = new nls::Real32($1); }
| REAL64        { $$ = new nls::Real64($1); }
;
%%
void nls::Parser::error (const location_type& l, const std::string& m)
{
    driver.error (l, m);
}
