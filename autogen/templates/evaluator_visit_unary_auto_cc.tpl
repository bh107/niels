#include <nls/ast/expr_unary_auto.hh>
#include <nls/ast/evaluator.hh>

using namespace std;
namespace nls {
namespace ast {

% for node in ast["nodes"]:
void Evaluator::visit(${node["class"]& node)
{
    walk()
    Variant in1 = pop();
    Variant res;

    // TODO: Derive type
    
    VType res_t = res->vtype();
    VType in1_t = in1->vtype();
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
}
%endfor

}}

