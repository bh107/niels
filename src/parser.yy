%require "3.0.2"
%define parse.error verbose
%{
#include <cinttypes>
#include <cstdint>
#include <climits>
#include <cstdio>
#include <cfloat>
#include <iostream>
#include <typeinfo>
#include <cstring>
#include <driver.hh>
#include <utils.hh>

using namespace std;

extern "C++" int yylex();
extern "C++" int yyparse(nls::Driver& env);
extern "C++" FILE *yyin;

extern "C++" void yyerror(nls::Driver& env, const char *s);
extern "C++" void assert_defined(nls::Driver& env, nls::Node* node);
extern "C++" void assert_undefined(nls::Driver& env, nls::Node* node);

%}
%parse-param { nls::Driver& env}
%union {
    nls::Node*   node;
    char* str;
}
%right <operator> ALIAS     "="
%right <operator> ASSIGN    "<-"
%right <operator> AS        "as"

%left <operator> ADD "+"
%left <operator> SUB "-"
%left <operator> MUL "*"
%left <operator> DIV "/"
%left <operator> MOD "%"
%left <operator> POW "^"

%left <operator> EQUAL          "=="
%left <operator> NOT_EQUAL      "!="
%left <operator> LTHAN          "<"
%left <operator> GTHAN          ">"
%left <operator> LTHAN_EQUAL    "<="
%left <operator> GTHAN_EQUAL    ">="

%left <operator> AND    "and"
%left <operator> OR     "or"
%left <operator> XOR    "xor"
%left <operator> NOT    "not"

%left <operator> BW_AND     "bw(and)"
%left <operator> BW_OR      "bw(or)"
%left <operator> BW_XOR     "bw(xor)"
%left <operator> BW_NOT     "bw(not)"
%left <operator> BW_LSHIFT  "lshift"
%left <operator> BW_RSHIFT  "rshift"

%right <operator> QUERY "?"
%right <operator> AT    "@"

%token IMPORT       "import"
%token NL           "newline"
%token LPAREN       "("
%token RPAREN       ")"
%token LBRACK       "["
%token RBRACK       "]"
%token LBRACE       "{"
%token RBRACE       "}"
%token COMMA        ","
%token COLON        ":"
%token SEMICOLON    ";"

%token <node> COMMENT BOOL INT32 INT64 REAL32 REAL64 STRING IDENT
%token RETURN FUNCTION WHILE WHEN IS OTHERWISE RECORD

%type <node> input block stmts stmt expr scalar val shape range ident
%type <node> args param params function function_block return
%type <node> while when is otherwise cases
%type <node> import
%type <node> record attr attrs noop
%type <node> scopeBegin scopeEnd

%%

input:
  %empty {}
| stmts {
    $$ = new nls::Collection(new nls::Ident("root"), $1);
    env.ast($$);
}
;

scopeBegin:
  %empty {
    env.scopeBegin();
}
;

scopeEnd:
  %empty {
    env.scopeEnd();
}
;

noop:
  %empty {
    $$ = new nls::Noop();
}
;

import:
  IMPORT ident {
    $$ = new nls::Import($2);
}
;

block:
  LBRACE noop[n] RBRACE {
    $$ = new nls::Block(new nls::Anon(), $n);
}
| LBRACE expr[e] RBRACE {
    $$ = new nls::Block(new nls::Anon(), $e);
}
| LBRACE stmt[s] RBRACE {
    $$ = new nls::Block(new nls::Anon(), $s);
}
| LBRACE stmts[s] RBRACE {
    $$ = new nls::Block(new nls::Anon(), $s);
}
;

return:
  RETURN expr NL {
    $$ = new nls::Return($2);
}
;

function_block:
  scopeBegin LPAREN params[p] RPAREN block[b] scopeEnd {
    $$ = new nls::FunctionDef($p, $b);
}
;

function:
  FUNCTION ident[id] function_block[fb] {
    assert_undefined(env, $id);
    
    $$ = new nls::Function($id, $fb);
    env.symbolTable()->put($id->str(), $$);
}
;

while:
  WHILE LPAREN expr RPAREN block {
    $$ = new nls::While($3, $5);
}
;

is:
  IS LPAREN scalar RPAREN block {
    $$ = new nls::Is($3, $5);
}
;
otherwise:
  OTHERWISE block {
    $$ = new nls::Otherwise(new nls::Bul(true), $2);
}
;
cases:
  is {
    $$ = new nls::Cases($1);
}
| cases is {
    $1->append(new nls::Cases($2));
    $$ = $1;
}
;
when:
  WHEN LPAREN expr RPAREN cases {
    $5->append(new nls::Cases(
        new nls::Otherwise(
            new nls::Bul(true),
            new nls::Block(new nls::Anon(), new nls::Noop())
        )
    ));
    $$ = new nls::When($3, $5);
}
| WHEN LPAREN expr RPAREN cases otherwise {
    $5->append(new nls::Cases($6));
    $$ = new nls::When($3, $5);
}
| WHEN LPAREN expr RPAREN block {
    $$ = new nls::WhenBool($3, new nls::Cases(
        new nls::Is(new nls::Bul(true), $5),
        new nls::Cases(new nls::Otherwise(
            new nls::Bul(true),
            new nls::Block(new nls::Anon(), new nls::Noop()))
        )));
}
| WHEN LPAREN expr RPAREN block otherwise {
    $$ = new nls::WhenBool($3, new nls::Cases(
        new nls::Is(new nls::Bul(true), $5),
        new nls::Cases($6)
    ));
}
;

stmts:
  stmt {
    $$ = new nls::StmtList($1);
    $$->left(NULL);
    $$->right($1);
}
| stmts stmt {
    if (env.fewerNoops() && ((typeid(*$2) == typeid(nls::Noop)))) {
        $$ = $1;
    } else {
        $$ = new nls::StmtList($1, $2);
    }
}
;

stmt:
  NL { $$ = new nls::Noop(); }
| COMMENT { $$ = $1; }
| expr NL { $$ = $1; }
| QUERY expr  {
    $$ = new nls::Query($2);
}
| ident ASSIGN expr {
    try {
        nls::Node* identNode = env.symbolTable()->getIdent($1);
        $$ = new nls::Assign($1, $3);
        if (!identNode) {
            env.symbolTable()->put($1->str(), $1);
        }
    } catch (exception& e) {
        yyerror(env, e.what());
    }
}
| ident ALIAS ident {
    assert_defined(env, $3);

    try {
        nls::Node* identNode = env.symbolTable()->getIdent($1);
        $$ = new nls::Alias($1, $3);
        if (!identNode) {   // Add Alias to symbol table
            env.symbolTable()->put($1->str(), $1);
        }
    } catch (exception& e) {
        yyerror(env, e.what());
    }
}
| function { $$ = $1; }
| record { $$ = $1; }
| return { $$ = $1; }
| when { $$ = $1; }
| while { $$ = $1; }
| block { $$ = $1; }
| import { $$ = $1; }
;

scalar:
  INT32  { $$ = $1; }
| INT64  { $$ = $1; }
| REAL32 { $$ = $1; }
| REAL64 { $$ = $1; }
| BOOL { $$ = $1; }
;

val:
  scalar { $$ = $1; }
| STRING { $$ = $1; }
;

shape:
  expr { $$ = new nls::Shape($1); }
| shape COMMA expr {
    $1->right(new nls::Shape($3));
    $$ = $1;
}
;

range:
  LBRACK expr SEMICOLON expr RBRACK {
    $$ = new nls::Range(false, false, $2, $4);
}
| LBRACK expr SEMICOLON expr LBRACK {
    $$ = new nls::Range(false, true, $2, $4);
}
| RBRACK expr SEMICOLON expr RBRACK {
    $$ = new nls::Range(true, false, $2, $4);
}
| RBRACK expr SEMICOLON expr LBRACK {
    $$ = new nls::Range(true, true, $2, $4);
}
| expr SEMICOLON expr {
    $$ = new nls::Range(false, false, $1, $3);
}
;

ident:
  IDENT {
    env.symbolTable()->getIdent($1);
    $$ = $1;
}
;

attr:
  ident[id] ASSIGN scalar[s] NL {
    assert_undefined(env, $id);
    $$ = new nls::Attr($id, $s);
    env.symbolTable()->put($id->str(), $$);
}
;

attrs:
  %empty { $$ = new nls::Empty(); }
| attr { $$ = new nls::AttrList($1); }
| attrs attr {
    $1->right(new nls::AttrList($2));
    $$ = $1;
}
;

record: RECORD ident[id] scopeBegin LBRACE NL attrs[a] RBRACE scopeEnd {
        assert_undefined(env, $id);
        
        $$ = new nls::RecDef($id, $a);
        env.symbolTable()->put($id->str(), $$);
    }
;

param: ident[id] {
        assert_undefined(env, $id);
        $$ = new nls::Param($1, new nls::Undefined());
        env.symbolTable()->put($1->str(), $$);
    }
    | ident[id] COLON scalar[s] {
        assert_undefined(env, $id);
        $$ = new nls::Param($1, $3);
        env.symbolTable()->put($1->str(), $$);
    }
;
params: %empty { $$ = new nls::Empty(); }
    | param { $$ = new nls::Params($1); }
    | params COMMA param {
        $1->right(new nls::Params($3));
        $$ = $1;
    }
;

args: %empty { $$ = new nls::Args(new nls::Empty()); }
    | expr { $$ = new nls::Args($1); }
    | args COMMA expr {
        $1->right(new nls::Args($3));
        $$ = $1;
    }
;

expr:
  val { $$ = $1; }
| range { $$ = $1; }
| ident {
    assert_defined(env, $1);
    $$ = $1;
}
| LPAREN expr RPAREN { $$ = $2; }
| ident LPAREN args RPAREN {
    assert_defined(env, $1);
    $$ = new nls::Call($1, $3);
}
| expr ADD expr  { $$ = new nls::Add($1, $3); }
| expr SUB expr  { $$ = new nls::Sub($1, $3); }
| expr MUL expr  { $$ = new nls::Mul($1, $3); }
| expr MOD expr  { $$ = new nls::Mod($1, $3); }
| expr DIV expr  { $$ = new nls::Div($1, $3); }
| expr POW expr  { $$ = new nls::Pow($1, $3); }

| expr EQUAL expr       { $$ = new nls::Equal($1, $3); }
| expr NOT_EQUAL expr   { $$ = new nls::NotEqual($1, $3); }
| expr LTHAN expr       { $$ = new nls::Lthan($1, $3); }
| expr GTHAN expr       { $$ = new nls::Gthan($1, $3); }
| expr LTHAN_EQUAL expr { $$ = new nls::LthanEqual($1, $3); }
| expr GTHAN_EQUAL expr { $$ = new nls::GthanEqual($1, $3); }

| expr AND expr { $$ = new nls::And($1, $3); }
| expr OR expr  { $$ = new nls::Or($1, $3); }
| expr XOR expr { $$ = new nls::Xor($1, $3); }
| NOT expr      { $$ = new nls::Not($2); }

| expr BW_AND expr  { $$ = new nls::BwAnd($1, $3); }
| expr BW_OR expr   { $$ = new nls::BwOr($1, $3); }
| expr BW_XOR expr  { $$ = new nls::BwXor($1, $3); }
| BW_NOT expr       { $$ = new nls::BwNot($2); }
| expr BW_LSHIFT expr { $$ = new nls::BwLshift($1, $3); }
| expr BW_RSHIFT expr { $$ = new nls::BwLshift($1, $3); }

| val AS LPAREN shape RPAREN {      $$ = new nls::As($1, $4);   }
| ident AS LPAREN shape RPAREN {    $$ = new nls::As($1, $4);   }
;

%%
void yyerror(nls::Driver& env, const char *s) {
    extern int yylineno;
    env.error(yylineno, s);
    exit(-1);
}

void assert_defined(nls::Driver& env, nls::Node* node) {
    if (!node->defined()) {
        stringstream ss;
        ss << "Identifier(" << node->str() << ") is not defined";
        yyerror(env, ss.str().c_str());
    }
}

void assert_undefined(nls::Driver& env, nls::Node* node) {
    if (node->defined()) {
        stringstream ss;
        ss << "Identifier(" << node->str() << ") used elsewhere.";
        yyerror(env, ss.str().c_str());
    }
}

