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

    Variant res;            // Allocate result
    Variant in1 = pop();    // Pop inputs
    Variant in2 = pop();

    res.value_type = in1.value_type > in2.value_type ? in1.value_type : in2.value_type;

    /*
    uint64_t mask = (res.value_type << 32) + (in1.value_type << 16) + in2.value_type;
    switch(mask) {
    case (NLS_R64_A << 32) + (NLS_R64_A << 16) + NLS_R64_A:
        res.value.r64_a = new bxx::multi_array<double>();
        *res.value.r64_a = bxx::bh_add(*in1.value.r64_a, *in2.value.r64_a);
        delete *in1.value.r64_a;
        delete *in2.value.r64_a;
        break;
    }
    */
    
    push(res);              // Push result
}
% endif
% endfor

}}

