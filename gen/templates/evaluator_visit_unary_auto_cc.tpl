#include <nls/ast/visitor/evaluator.hh>

using namespace std;
namespace nls {
namespace ast {

%for k, op, ninput, exprs in operators:
void Evaluator::visit(${op2node[op]}& node)
{
    walk();
    Variant in1 = pop();
    Variant res;

    // TODO: Derive type of res, 
    
    ValueType res_t = res.value_type;
    ValueType in1_t = in1.value_type;
    uint64_t mask = (res_t << 16) + in1_t;
    switch(mask) {
    
    <%
    expr, sigs = exprs["kk"]
    %>
    %for res_vtype, in1_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 16) + ${vtype2enum[in1_vtype]}:
        ${expr.format(res_t=res_vtype, in1_t=in1_vtype)};
        break;
    %endfor

    <%
    expr, sigs = exprs["aa"]
    %>
    %for res_vtype, in1_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 16) + ${vtype2enum[in1_vtype]}:
        ${expr.format(res_t=res_vtype, in1_t=in1_vtype)};
        break;
    %endfor

    default:
        throw std::logic_error("Unsupported types for operator.");
        break;
    }

    push(res);
}
%endfor

}}

