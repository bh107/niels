#include <iostream>
#include <nls/ast/exception.hh>
#include <nls/ast/visitor/evaluator.hh>
#include <nls/utils.hh>

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

void Evaluator::visit(Query& node)
{
    walk(node);

    Variant var = pop();
    cout << "? " << var.value.r64 << endl;
}


}}
