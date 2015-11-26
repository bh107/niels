#include <iostream>
#include <nls/ast/visitor/evaluator.hh>

using namespace std;

namespace nls {
namespace ast {

Evaluator::Evaluator(SymbolTable& symbolTable) : _symbolTable(symbolTable) { }

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

void Evaluator::visit(Node &node)
{
    walk(node);
}

void Evaluator::visit(R32 &node)
{
    walk(node);

    push(*node.variant());
}

void Evaluator::visit(Add &node)
{
    walk(node);

    Variant right = pop();
    Variant left = pop();

    Variant res;
    res.value.r32 = left.value.r32 + right.value.r32;

    push(res);

    cout << "Evaluator::visit(Add& node)" << endl;
    cout << left.value.r32 << " + " << right.value.r32 << endl;
    cout << res.value.r32 << endl;
}

void Evaluator::visit(Sub &node)
{
    walk(node);

    Variant right = pop();
    Variant left = pop();

    Variant res;
    res.value.r32 = left.value.r32 - right.value.r32;
    push(res);

    cout << "Evaluator::visit(Sub& node)" << endl;
    cout << left.value.r32 << " - " << right.value.r32 << endl;
    cout << res.value.r32 << endl;
}

}}