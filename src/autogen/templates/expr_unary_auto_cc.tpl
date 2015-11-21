#include <nls/ast/expr_unary_auto.hh>

using namespace std;
namespace nls {
namespace ast {

%for k, op, ninput, exprs in operators:

${op2node[op]}::${op2node[op]}(Node* left) : Node(left)
{
    %if k == "comparison" or k == "logical":
    vtype(NLS_BUL);
    %elif k == "arithmetic" or k == "bitwise":
    _vtype = left->vtype();
    %else:
    FORGOT SOMETHING
    %endif

    stype(EXPR);
}
void ${op2node[op]}::eval(Driver& env)
{
    Node* res = this;
    Node* in1 = left();
    
    VType res_t = res->vtype(); // Evaluate *this
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
string ${op2node[op]}::dot_label(void) { return "${op2node[op]}"; }
%endfor

}}

