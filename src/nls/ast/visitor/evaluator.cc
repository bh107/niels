#include <iostream>
#include <nls/ast/visitor/evaluator.hh>

using namespace std;

namespace nls {
namespace ast {

// Constructor
Evaluator::Evaluator(void)
    : Visitor()
{
}
Evaluator::Evaluator(SymbolTable& symbolTable)
    : Visitor(), _symbolTable(symbolTable)
{
}

Evaluator::~Evaluator(void)
{
}

// Stack operations
Variant Evaluator::pop(void)
{
    Variant res = _stack.top();
    _stack.pop();
    return res;
}

void Evaluator::push(Variant var)
{
    _stack.push(var);
}

// Visitors



}}
