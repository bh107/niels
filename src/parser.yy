%require "3.0.2"
%{
#include <cinttypes>
#include <cstdint>
#include <climits>
#include <cstdio>
#include <cfloat>
#include <iostream>
#include <typeinfo>
#include <cstring>
#include "ast.hh"
#include "symboltable.hh"
#include "utils.hh"

using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

nir::Node* program;             // Root of the AST
nir::SymbolTable* symbolTable;  // Root of the SymbolTable
bool fewerNoops = true;         // Whether or not Noop nodes should be removed

extern "C" void yyerror(const char *s);
extern "C" void assert_defined(nir::Node* node);
extern "C" void assert_undefined(nir::Node* node);
%}
%define parse.error verbose
%union {
    nir::Node*   node;
    char* str;
}

%right <operator> ALIAS ASSIGN

%right <operator> AS

%left <operator> ADD SUB
%left <operator> MUL DIV MOD POW

%left <operator> LTHAN GTHAN
%left <operator> EQUAL NOT_EQUAL
%left <operator> LTHAN_EQUAL
%left <operator> GTHAN_EQUAL

%left <operator> OR XOR AND

%right <operator> INVERT NEGATE QUERY
%token <operator> AT

%token IMPORT
%token NL
%token LPAREN RPAREN 
%token LBRACK RBRACK
%token LBRACE RBRACE
%token COMMA COLON SEMICOLON

%token <node> COMMENT
%token <node> INT32 INT64 REAL32 REAL64 STRING BOOL IDENT
%token RETURN FUNCTION WHILE WHEN IS OTHERWISE RECORD

%type <node> input block stmts stmt expr scalar val shape range ident
%type <node> args param params function function_block return
%type <node> while when is otherwise cases
%type <node> import
%type <node> record attr attrs noop
%type <node> scopeBegin scopeEnd

%%

input: %empty { }
    | stmts {
        $$ = new nir::Collection(new nir::Ident("root"), $1);
        program = $$;
    }
;

scopeBegin: %empty {
        nir::SymbolTable* childScope = new nir::SymbolTable();
        childScope->parent(symbolTable);

        if (symbolTable->child()) {
            nir::SymbolTable* sibling = symbolTable->child();
            while(sibling->sibling()) {
                sibling = sibling->sibling();
            }
            sibling->sibling(childScope);
        } else {
            symbolTable->child(childScope);
        }
        symbolTable = childScope;
    }
;

scopeEnd: %empty {
        symbolTable = symbolTable->parent();
    }
;

noop: %empty {
        $$ = new nir::Noop();
    }
;

import: IMPORT ident {
        $$ = new nir::Import($2);
    }
;

block: LBRACE noop[n] RBRACE {
        $$ = new nir::Block(new nir::Anon(), $n);
    }
    | LBRACE expr[e] RBRACE {
        $$ = new nir::Block(new nir::Anon(), $e);
    }
    | LBRACE stmt[s] RBRACE {
        $$ = new nir::Block(new nir::Anon(), $s);
    }
    | LBRACE stmts[s] RBRACE {
        $$ = new nir::Block(new nir::Anon(), $s);
    }
;

return: RETURN expr NL {
        $$ = new nir::Return($2);
    }
;

function_block: scopeBegin LPAREN params[p] RPAREN block[b] scopeEnd {
        $$ = new nir::FunctionDef($p, $b);
    }
;

function: FUNCTION ident[id] function_block[fb] {
        assert_undefined($id);
        
        $$ = new nir::Function($id, $fb);
        symbolTable->put($id->str(), $$);
    }
;

while: WHILE LPAREN expr RPAREN block {
        $$ = new nir::While($3, $5);
    }
;

is: IS LPAREN scalar RPAREN block {
        $$ = new nir::Is($3, $5);
    }
;
otherwise: OTHERWISE block {
        $$ = new nir::Otherwise(new nir::Bool(true), $2);
    }
;
cases:  is {
        $$ = new nir::Cases($1);
    }
    | cases is {
        $1->append(new nir::Cases($2));
        $$ = $1;
    }
;
when: WHEN LPAREN expr RPAREN cases {
        $5->append(new nir::Cases(
            new nir::Otherwise(
                new nir::Bool(true),
                new nir::Block(new nir::Anon(), new nir::Noop())
            )
        ));
        $$ = new nir::When($3, $5);
    }
    | WHEN LPAREN expr RPAREN cases otherwise {
        $5->append(new nir::Cases($6));
        $$ = new nir::When($3, $5);
    }
    | WHEN LPAREN expr RPAREN block {
        $$ = new nir::WhenBool($3, new nir::Cases(
            new nir::Is(new nir::Bool(true), $5),
            new nir::Cases(new nir::Otherwise(
                new nir::Bool(true),
                new nir::Block(new nir::Anon(), new nir::Noop()))
            )));
    }
    | WHEN LPAREN expr RPAREN block otherwise {
        $$ = new nir::WhenBool($3, new nir::Cases(
            new nir::Is(new nir::Bool(true), $5),
            new nir::Cases($6)
        ));

    }
;

stmts: stmt {
        $$ = new nir::StmtList($1);
        $$->left(NULL);
        $$->right($1);
    }
    | stmts stmt {
        if (fewerNoops && ((typeid(*$2) == typeid(nir::Noop)))) {
            $$ = $1;
        } else {
            $$ = new nir::StmtList($1, $2);
        }
    }
;

stmt: NL { $$ = new nir::Noop(); }
    | COMMENT { $$ = $1; }
    | expr NL { $$ = $1; }
    | QUERY expr  {
        $$ = new nir::Query($2);
        $$->eval();
    }
    | ident ASSIGN expr {
        try {
            nir::Node* identNode = symbolTable->getIdent($1);
            $$ = new nir::Assign($1, $3);
            if (!identNode) {
                symbolTable->put($1->str(), $1);
            }
        } catch (exception& e) {
            yyerror(e.what());
        }
    }
    | ident ALIAS ident {
        assert_defined($3);

        try {
            nir::Node* identNode = symbolTable->getIdent($1);
            $$ = new nir::Alias($1, $3);
            if (!identNode) {   // Add Alias to symbol table
                symbolTable->put($1->str(), $1);
            }
        } catch (exception& e) {
            yyerror(e.what());
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

scalar: INT32  { $$ = $1; }
    | INT64  { $$ = $1; }
    | REAL32 { $$ = $1; }
    | REAL64 { $$ = $1; }
    | BOOL { $$ = $1; }
;
val: scalar { $$ = $1; }
    | STRING { $$ = $1; }
;
shape: expr { $$ = new nir::Shape($1); }
    | shape COMMA expr {
        $1->right(new nir::Shape($3));
        $$ = $1;
    }
;
range: LBRACK expr SEMICOLON expr RBRACK {
        $$ = new nir::Range(false, false, $2, $4);
    }
    | LBRACK expr SEMICOLON expr LBRACK {
        $$ = new nir::Range(false, true, $2, $4);
    }
    | RBRACK expr SEMICOLON expr RBRACK {
        $$ = new nir::Range(true, false, $2, $4);
    }
    | RBRACK expr SEMICOLON expr LBRACK {
        $$ = new nir::Range(true, true, $2, $4);
    }
    | expr SEMICOLON expr {
        $$ = new nir::Range(false, false, $1, $3);
    }
;

ident: IDENT {
        symbolTable->getIdent($1);
        $$ = $1;
    }
;

attr: ident[id] ASSIGN scalar[s] NL {
        assert_undefined($id);
        $$ = new nir::Attr($id, $s);
        symbolTable->put($id->str(), $$);
    }
;
attrs: %empty { $$ = new nir::Empty(); }
    | attr { $$ = new nir::AttrList($1); }
    | attrs attr {
        $1->right(new nir::AttrList($2));
        $$ = $1;
    }
;

record: RECORD ident[id] scopeBegin LBRACE NL attrs[a] RBRACE scopeEnd {
        assert_undefined($id);
        
        $$ = new nir::RecDef($id, $a);
        symbolTable->put($id->str(), $$);
    }
;

param: ident[id] {
        assert_undefined($id);
        $$ = new nir::Param($1, new nir::Undefined());
        symbolTable->put($1->str(), $$);
    }
    | ident[id] COLON scalar[s] {
        assert_undefined($id);
        $$ = new nir::Param($1, $3);
        symbolTable->put($1->str(), $$);
    }
;
params: %empty { $$ = new nir::Empty(); }
    | param { $$ = new nir::Params($1); }
    | params COMMA param {
        $1->right(new nir::Params($3));
        $$ = $1;
    }
;

args: %empty { $$ = new nir::Args(new nir::Empty()); }
    | expr { $$ = new nir::Args($1); }
    | args COMMA expr {
        $1->right(new nir::Args($3));
        $$ = $1;
    }
;

expr: val { $$ = $1; }
    | range { $$ = $1; }
    | ident {
        assert_defined($1);
        $$ = $1;
    }
    | LPAREN expr RPAREN { $$ = $2; }
    | ident LPAREN args RPAREN {
        assert_defined($1);
        $$ = new nir::Call($1, $3);
    }
    | expr ADD expr  { $$ = new nir::Add($1, $3); }
    | expr SUB expr  { $$ = new nir::Sub($1, $3); }
    | expr MUL expr  { $$ = new nir::Mul($1, $3); }
    | expr MOD expr  { $$ = new nir::Mod($1, $3); }
    | expr DIV expr  { $$ = new nir::Div($1, $3); }
    | expr POW expr  { $$ = new nir::Pow($1, $3); }
    | expr LTHAN expr  { $$ = new nir::LThan($1, $3); }
    | NEGATE expr { $$ = new nir::Neg($2); }
    | INVERT expr { $$ = new nir::Inv($2); }
    | val AS LPAREN shape RPAREN {
        $$ = new nir::As($1, $4);
    }
    | ident AS LPAREN shape RPAREN {
        $$ = new nir::As($1, $4);
    }
;

%%
void yyerror(const char *s) {
    extern int yylineno;
    cout << "Error near line " << yylineno << ": " << s << endl;
    exit(-1);
}

void assert_defined(nir::Node* node) {
    if (!node->defined()) {
        stringstream ss;
        ss << "Identifier(" << node->str() << ") is not defined";
        yyerror(ss.str().c_str());
    }
}

void assert_undefined(nir::Node* node) {
    if (node->defined()) {
        stringstream ss;
        ss << "Identifier(" << node->str() << ") used elsewhere.";
        yyerror(ss.str().c_str());
    }
}

int main(int, char**) {
    FILE *myfile = fopen("input.nls", "r");
    if (!myfile) {
        cout << "I cannot open input.nls!" << endl;
        return -1;
    }
    
    symbolTable = new nir::SymbolTable();
    yyin = myfile;  // Switch from STDIN to 'input.nls'
    do {
        yyparse();
    } while (!feof(yyin));

    /*
    cout << "graph {" << endl;
    cout << "graph[ordering=out]" << endl;
    
    while(symbolTable->parent()) {  // Go to top-level
        symbolTable = symbolTable->parent();
    }
    cout << nir::dot(symbolTable) << endl;

    cout << nir::dot(program) << endl;

    cout << "}" << endl;
    */
}
