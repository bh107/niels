%define parse.error verbose
%code requires {
#include <cinttypes>
#include <cstdint>
#include <climits>
#include <cstdio>
#include <cfloat>
#include <iostream>
#include <typeinfo>
#include <cstring>
#include <nls/nls.hh>

using namespace std;

extern "C++" int yylex(nls::Driver& env);
extern "C++" int yyparse(nls::Driver& env);
extern "C++" FILE *yyin;

extern "C++" void yyerror(nls::Driver& env, const char *s);

}
%param { nls::Driver& env }
%union {
    nls::ast::Node*   node;
    char* str;
}

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

%right <operator> AS "as"
%right <operator> ALIAS     "="
%right <operator> ASSIGN    "<-"

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
%token DOTDOT       ".."
%token DOT          "."
%token <node> COMMENT BOOL INT32 INT64 REAL32 REAL64
%token <node> COMPLEX64 COMPLEX128
%token <node> STRING IDENT
%token RETURN FUNCTION WHILE CONTINUE BREAK WHEN IS OTHERWISE
%token NEW RECORD

%left LPAREN

%type <node> input
%type <node> scalar
%type <node> val
%type <node> shape
%type <node> range
%type <node> ident
%type <node> noop
%type <node> expr
%type <node> block
%type <node> stmt_list
%type <node> stmt
%type <node> return
%type <node> while while_body
%type <node> when is otherwise case_list
%type <node> import
%type <node> arg_list
%type <node> function function_head function_body param_list param
%type <node> record record_head record_body attr attrs
%type <node> accessor
%type <operator> exitScope
%%

input:
  %empty {
    $$ = NULL;
}
| stmt_list {
    $$ = new nls::ast::Module(new nls::ast::Ident("root"), $1);
    env.ast($$);
}
;

exitScope:
  %empty {
    env.exitScope();
}
;

noop:
  %empty {
    $$ = new nls::ast::Noop();
}
;

import:
  IMPORT ident {
    $$ = new nls::ast::Import($2);
}
;

block:
  LBRACE noop[n] RBRACE {
    $$ = new nls::ast::Block(new nls::ast::Anon(), $n);
}
| LBRACE expr[e] RBRACE {
    $$ = new nls::ast::Block(new nls::ast::Anon(), $e);
}
| LBRACE stmt_list[s] RBRACE {
    $$ = new nls::ast::Block(new nls::ast::Anon(), $s);
}
;

return:
  RETURN expr NL {
    $$ = new nls::ast::Return($2);
}
;

function_body:
  LPAREN param_list[p] RPAREN block[b] {
    $$ = new nls::ast::FunctionBody($p, $b);
}
;

function_head:
    FUNCTION ident[id] {

    $$ = new nls::ast::FunctionDef($id);
}

function:
  function_head[h] function_body[b] exitScope {
    $h->right($b);
    $$ = $h;
}
;

while_body:
    block[b] {
    
    nls::ast::Node* c = new nls::ast::StmtList(
        $b->right(),
        new nls::ast::Continue()
    );
    $b->right(c);
}
;

while:
  WHILE LPAREN expr[e] RPAREN while_body[b] {
    $$ = new nls::ast::While($e, $b);
}
;

is:
  IS LPAREN scalar RPAREN block {
    $$ = new nls::ast::Is($3, $5);
}
;
otherwise:
  OTHERWISE block {
    $$ = new nls::ast::Otherwise(new nls::ast::Bul("true"), $2);
}
;
case_list:
  is {
    $$ = new nls::ast::CaseList($1);
}
| case_list is {
    $1->append(new nls::ast::CaseList($2));
    $$ = $1;
}
;
when:
  WHEN LPAREN expr RPAREN case_list {
    $5->append(new nls::ast::CaseList(
        new nls::ast::Otherwise(
            new nls::ast::Bul("true"),
            new nls::ast::Block(new nls::ast::Anon(), new nls::ast::Noop())
        )
    ));
    $$ = new nls::ast::When($3, $5);
}
| WHEN LPAREN expr RPAREN case_list otherwise {
    $5->append(new nls::ast::CaseList($6));
    $$ = new nls::ast::When($3, $5);
}
| WHEN LPAREN expr RPAREN block {
    $$ = new nls::ast::When($3, new nls::ast::CaseList(
        new nls::ast::Is(new nls::ast::Bul("true"), $5),
        new nls::ast::CaseList(new nls::ast::Otherwise(
            new nls::ast::Bul("true"),
            new nls::ast::Block(new nls::ast::Anon(), new nls::ast::Noop()))
        )));
}
| WHEN LPAREN expr RPAREN block otherwise {
    $$ = new nls::ast::When($3, new nls::ast::CaseList(
        new nls::ast::Is(new nls::ast::Bul("true"), $5),
        new nls::ast::CaseList($6)
    ));
}
;

scalar:
  INT32  { $$ = $1; }
| INT64  { $$ = $1; }
| REAL32 { $$ = $1; }
| REAL64 { $$ = $1; }
| COMPLEX64 { $$ = $1; }
| COMPLEX128 { $$ = $1; }
| BOOL { $$ = $1; }
;

val:
  scalar { $$ = $1; }
| STRING { $$ = $1; }
;

shape:
  expr[e] { $$ = new nls::ast::Shape($e); }
| shape[s] COMMA expr[e] {
    $s->append(new nls::ast::Shape($e));
    $$ = $s;
}
;

range:
  LBRACK expr DOTDOT expr RBRACK {
    //$$ = new nls::ast::Range(false, false, $2, $4);
    $$ = new nls::ast::Range($2, $4);
}
| LBRACK expr DOTDOT expr LBRACK {
    //$$ = new nls::ast::Range(false, false, $2, $4);
    $$ = new nls::ast::Range($2, $4);
}
| RBRACK expr DOTDOT expr RBRACK {
    //$$ = new nls::ast::Range(false, false, $2, $4);
    $$ = new nls::ast::Range($2, $4);
}
| RBRACK expr DOTDOT expr LBRACK {
    //$$ = new nls::ast::Range(false, false, $2, $4);
    $$ = new nls::ast::Range($2, $4);
}
;

ident:
  IDENT {
    $$ = $1;
}
;

attr:
  ident[id] ASSIGN scalar[s] {
    $$ = new nls::ast::Attr($id, $s);
}
;

attrs:
  %empty { $$ = new nls::ast::Empty(); }
| attr { $$ = new nls::ast::AttrList($1); }
| attrs NL attr {
    $1->right(new nls::ast::AttrList($3));
    $$ = $1;
}
;

record_body:
  LBRACE NL attrs[a] NL RBRACE {
    $$ = $a;
}
;

record_head:
  RECORD ident[id] {
    $$ = new nls::ast::RecordDef($id);
}
;

record:
  record_head[r] record_body[b] exitScope {
    $r->right($b);
    $$ = $r;
}
;

param: ident[id] COLON scalar[s] {
        $$ = new nls::ast::Param($1, $3);
    }
;
param_list: %empty { $$ = new nls::ast::Empty(); }
    | param { $$ = new nls::ast::ParamList($1); }
    | param_list COMMA param {
        $1->right(new nls::ast::ParamList($3));
        $$ = $1;
    }
;

arg_list: %empty { $$ = new nls::ast::ArgList(new nls::ast::Empty()); }
    | expr { $$ = new nls::ast::ArgList($1); }
    | arg_list COMMA expr {
        $1->right(new nls::ast::ArgList($3));
        $$ = $1;
    }
;

accessor:
  ident DOT ident {
    $$ = new nls::ast::Accessor($1, $3);
}
| accessor DOT ident {
    $1->left()->name() += "::"+ $1->right()->name();
    $1->right($3);
    $$ = $1; 
}
;

expr:
  val { $$ = $1; }
| ident {
    $$ = $1;
}
| range { $$ = $1; }
| expr LPAREN arg_list RPAREN {
    $$ = new nls::ast::Call($1, $3);
}
| NEW ident { $$ = new nls::ast::Record($2); }
| accessor { $$ = $1; }
| LPAREN expr RPAREN { $$ = $2; }
| expr ADD expr  { $$ = new nls::ast::Add($1, $3); }
| expr SUB expr  { $$ = new nls::ast::Sub($1, $3); }
| expr MUL expr  { $$ = new nls::ast::Mul($1, $3); }
| expr MOD expr  { $$ = new nls::ast::Mod($1, $3); }
| expr DIV expr  { $$ = new nls::ast::Div($1, $3); }
| expr POW expr  { $$ = new nls::ast::Pow($1, $3); }
| expr EQUAL expr       { $$ = new nls::ast::Equal($1, $3); }
| expr NOT_EQUAL expr   { $$ = new nls::ast::NotEqual($1, $3); }
| expr LTHAN expr       { $$ = new nls::ast::Lthan($1, $3); }
| expr GTHAN expr       { $$ = new nls::ast::Gthan($1, $3); }
| expr LTHAN_EQUAL expr { $$ = new nls::ast::LthanEqual($1, $3); }
| expr GTHAN_EQUAL expr { $$ = new nls::ast::GthanEqual($1, $3); }

| expr AND expr { $$ = new nls::ast::And($1, $3); }
| expr OR expr  { $$ = new nls::ast::Or($1, $3); }
| expr XOR expr { $$ = new nls::ast::Xor($1, $3); }
| NOT expr      { $$ = new nls::ast::Not($2); }

| expr BW_AND expr  { $$ = new nls::ast::BwAnd($1, $3); }
| expr BW_OR expr   { $$ = new nls::ast::BwOr($1, $3); }
| expr BW_XOR expr  { $$ = new nls::ast::BwXor($1, $3); }
| BW_NOT expr       { $$ = new nls::ast::BwNot($2); }
| expr BW_LSHIFT expr { $$ = new nls::ast::BwLshift($1, $3); }
| expr BW_RSHIFT expr { $$ = new nls::ast::BwLshift($1, $3); }

| expr AS LPAREN shape RPAREN { $$ = new nls::ast::As($1, $4); }
;

stmt_list:
  stmt {
    $$ = new nls::ast::StmtList($1);
    $$->left(NULL);
    $$->right($1);
}
| stmt_list stmt {
    if (env.fewerNoops() && ((typeid(*$2) == typeid(nls::ast::Noop)))) {
        $$ = $1;
    } else {
        $$ = new nls::ast::StmtList($1, $2);
    }
}
;

stmt:
  NL { $$ = new nls::ast::Noop(); }
| COMMENT { $$ = $1; }
| QUERY expr  {
    $$ = new nls::ast::Query($2);
}
| expr NL { $$ = $1; }
| ident ASSIGN expr {
    $$ = new nls::ast::Assign($1, $3);
}
| ident ALIAS ident {
    $$ = new nls::ast::Alias($1, $3);
}
| function { $$ = $1; }
| record { $$ = $1; }
| return { $$ = $1; }
| while { $$ = $1; }
| BREAK { $$ = new nls::ast::Break(); }
| CONTINUE { $$ = new nls::ast::Continue(); }
| when { $$ = $1; }
| block { $$ = $1; }
| import { $$ = $1; }
;

%%
void yyerror(nls::Driver& env, const char *s) {
    extern int yylineno;
    env.error(yylineno, s);
    exit(-1);
}

/*
void assert_known(nls::Driver& env, nls::ast::Node* node) {
    if (!node->known()) {
        stringstream ss;
        ss << "Identifier(" << node->name() << ") is not defined";
        yyerror(env, ss.str().c_str());
    }
}

void assert_undefined(nls::Driver& env, nls::ast::Node* node) {
    if (node->defined()) {
        stringstream ss;
        ss << "Identifier(" << node->name() << ") used elsewhere.";
        yyerror(env, ss.str().c_str());
    }
}*/

