#include <nls/ast/visitor/evaluator.hh>

using namespace std;
namespace nls {
namespace ast {

% for k, op, ninput, exprs in operators:
void Evaluator::visit(${op2node[op]}& node)
{
    walk()

    push(*_variant);
}
%endfor

}}

