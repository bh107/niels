#include <parser.hh>
#include <nls/driver.hh>
#include <nls/utils.hh>
#include <nls/ast/visitor/evaluator.hh>

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

/*
void Driver::walk(ast::Node* node)
{
    if (NULL==node) {
        return;
    }

    if ((typeid(*node) == typeid(ast::WhenBool)) or (typeid(*node) == typeid(ast::When))) {
        walk(node->left());
        nls::ast::Node* cases = node;
        while(cases->right()) {
            cases = cases->right();
            ast::Node* caseNode = cases->left();
            if (typeid(*caseNode) == typeid(ast::Otherwise)) {
                walk(caseNode->right());
                break;
            } else {
                if (caseNode->left()->value().r64 == node->left()->value().r64) {
                    walk(caseNode->right());
                    break;
                }
            }
        }
    } else if (typeid(*node) == typeid(ast::While)) {
        ast::Node* condNode = node->left();
        walk(condNode);
        while(condNode->value().bul) {
            walk(node->right());
            walk(condNode);
        }
    } else if (typeid(*node) == typeid(ast::As)) {
        walk(node->left());
        node->eval(*this);
    } else if (typeid(*node) == typeid(ast::Query)) {
        walk(node->left());
        node->eval(*this);
    } else {
        walk(node->left());
        walk(node->right());
        node->eval(*this);
    }
}*/

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

void Driver::createScope(const std::string& val)
{
    _symbolTable.scope(_symbolTable.scope()+"::"+val);
}

void Driver::exitScope(void)
{
    string parentScope = _symbolTable.scope();
    size_t found = parentScope.rfind("::");
    if (found) {
        parentScope = parentScope.substr(0, found);
        _symbolTable.scope(parentScope);
    }
}

SymbolTable& Driver::symbolTable(void)
{
    return _symbolTable;
}

void Driver::ast(ast::Node* node)
{
    _ast = node;
}

ast::Node* Driver::ast(void)
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

void Driver::eval(void)
{
    nls::ast::Evaluator evaluator(symbolTable());
    ast()->accept(evaluator);
}

string Driver::dot(void)
{
    stringstream ss;                        // CReate dot
    ss << "graph {" << endl;
    ss << "graph[ordering=out]" << endl;
    ss << nls::dot(ast()) << endl;
    ss << "}" << endl;

    return ss.str();
}

}

