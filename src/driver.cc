#include <utils.hh>
#include <driver.hh>
#include <parser.hh>

using namespace std;
namespace nls {

Driver::Driver(void)
    : _trace_scanning(false), _trace_parsing(false), _fewer_noops(true)
{
}

Driver::Driver(bool trace_scanning, bool trace_parsing, bool fewer_noops)
    : _trace_scanning(trace_scanning), _trace_parsing(trace_parsing), _fewer_noops(fewer_noops)
{
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

void Driver::eval(Node* node)
{
    if (NULL==node) {
        return;
    }
    eval(node->left());
    eval(node->right());
    cout << "Evaluating: " << node->dot_label() << endl;
    node->eval();
    cout << node->txt() << endl;
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
    _symbolTable.scope(_symbolTable.scope()+".");
}

void Driver::scopeEnd(void)
{
}

SymbolTable& Driver::symbolTable(void)
{
    return _symbolTable;
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

string Driver::dot(void)
{
    stringstream ss;                        // CReate dot
    ss << "graph {" << endl;
    ss << "graph[ordering=out]" << endl;
    ss << symbolTable().dot() << endl;
    ss << nls::dot(ast()) << endl;
    ss << "}" << endl;

    return ss.str();
}

}

