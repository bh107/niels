#include <ast.hh>

using namespace std;
namespace nls {

%for name, aname, expr, sigs, ktype in operators:

${aname}::${aname}(Node* left) : Node(left)
{
    %if ktype == "comparison" or ktype == "logical":
    vtype(NLS_BUL);
    %elif ktype == "arithmetic" or ktype == "bitwise":
    _vtype = left->vtype();
    %else:
    FORGOT SOMETHING
    %endif

    stype(EXPR);
}
void ${aname}::eval(Driver& env)
{
    Node* res = this;
    Node* in1 = left();
    
    VType res_t = res->vtype(); // Evaluate *this
    VType in1_t = in1->vtype();
    uint64_t mask = (res_t << 16) + in1_t;
    switch(mask) {
    %for res_vtype, in1_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 16) + ${vtype2enum[in1_vtype]}:
        res->value().${res_vtype} = ${expr.format(in1_t=in1_vtype)};
        break;
    %endfor

    default:
        throw std::logic_error("Unsupported types for operator.");
        break;
    }
}
string ${aname}::dot_label(void) { return "${aname}"; }
%endfor

}

