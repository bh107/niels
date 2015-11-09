#include <driver.hh>
#include <parser.hh>

using namespace std;
namespace nls {

Driver::Driver(void)
    : _trace_scanning(false), _trace_parsing(false), _fewer_noops(true)
{
    _symbolTable = new SymbolTable();
}

Driver::Driver(bool trace_scanning, bool trace_parsing, bool fewer_noops)
    : _trace_scanning(trace_scanning), _trace_parsing(trace_parsing), _fewer_noops(fewer_noops)
{
    _symbolTable = new SymbolTable();
}

Driver::~Driver(void)
{

}

int Driver::parse(const string& filename)
{
    _filename = filename;
    scanBegin ();
    int res = yyparse(*this);
    scanEnd ();

    return res;
}

//
// NOTE: scanBegin and scanEnd are defined in scanner.ll
//

void Driver::error(const std::string& m)
{
    std::cerr << m << std::endl;
}

void Driver::error(int yylineno, const std::string& m)
{
    std::cerr << "Error near line " << yylineno << ": " << m << endl;
}



void Driver::scopeBegin(void)
{
    SymbolTable* childScope = new nls::SymbolTable();
    childScope->parent(_symbolTable);

    if (_symbolTable->child()) {
        SymbolTable* sibling = _symbolTable->child();
        while(sibling->sibling()) {
            sibling = sibling->sibling();
        }
        sibling->sibling(childScope);
    } else {
        _symbolTable->child(childScope);
    }
    _symbolTable = childScope;
}

void Driver::scopeEnd(void)
{
    _symbolTable = _symbolTable->parent();
}

SymbolTable* Driver::symbolTable(void)
{
    return _symbolTable;
}

void Driver::symbolTable(SymbolTable* table)
{
    _symbolTable = table;
}

void Driver::ast(Node* node)
{
    _ast = node;
}

Node* Driver::ast(void)
{
    return _ast;
}

bool Driver::traceScanning(void)
{
    return _trace_scanning;
}

bool Driver::traceParsing(void)
{
    return _trace_parsing;
}

bool Driver::fewerNoops(void)
{
    return _fewer_noops;
}

}

