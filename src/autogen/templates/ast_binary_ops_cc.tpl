#include <ast.hh>
#include <utils.hh>

using namespace std;
namespace nls {

string op_vtype_error(Node* res, Node* left)
{
    stringstream ss;
    ss << "Unsupported types for operator: ";
    ss << VType_text(res->vtype())      << ", ";
    ss << VType_text(left->vtype())     << ".";
    return ss.str();
}

string op_vtype_error(Node* res, Node* left, Node* right)
{
    stringstream ss;
    ss << "Unsupported types for operator: ";
    ss << VType_text(res->vtype())      << ", ";
    ss << VType_text(left->vtype())     << ", ";
    ss << VType_text(right->vtype())    << ".";
    return ss.str();
}

%for name, aname, expr, sigs, ktype in operators:

${aname}::${aname}(Node* left, Node* right) : Node(left, right)
{
    %if ktype == "comparison" or ktype == "logical":
    vtype(NLS_BUL);
    %elif ktype == "arithmetic" or ktype == "bitwise":
    _vtype = left->vtype() >= right->vtype() ? left->vtype() : right->vtype();
    %else:
    FORGOT SOMETHING
    %endif
    stype(EXPR);
}
void ${aname}::eval(Driver& env)
{
    Node* res = this;
    Node* in1 = left();
    Node* in2 = right();

    VType res_t = res->vtype(); // Evaluate *this
    VType in1_t = in1->vtype();
    VType in2_t = in2->vtype();
    uint64_t mask = (res_t << 32) + (in1_t << 16) + in2_t;
    switch(mask) {
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        res->value().${res_vtype} = ${expr.format(in1_t=in1_vtype, in2_t=in2_vtype)};
        break;
    %endfor
    
    default:
        throw std::logic_error(op_vtype_error(res, in1, in2));
        break;
    }

}
string ${aname}::dot_label(void) { return "${aname}"; }
%endfor

}

