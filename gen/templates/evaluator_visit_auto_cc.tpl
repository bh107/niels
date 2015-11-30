#include <nls/ast/visitor/evaluator.hh>

using namespace std;
namespace nls {
namespace ast {

% for node in nodes:
% if node["evaluator"] == "default":
void Evaluator::visit(${node["class"]}& node)
{
    walk(node);
}
% elif node["evaluator"] == "variant":
void Evaluator::visit(${node["class"]}& node)
{
    walk(node);
    push(*node.variant());
}
%elif node["evaluator"] == "expr":
void Evaluator::visit(${node["class"]}& node)
{
    walk(node);             // Evaluate children

    Variant right = pop();  // Pop results
    Variant left = pop();
    Variant res;            // Allocate result

                            // Compute it
    res.value.r64 = left.value.r64 + right.value.r64;
    res.value_type = NLS_R64;

    push(res);              // Push result
}
% endif
% endfor

}}

