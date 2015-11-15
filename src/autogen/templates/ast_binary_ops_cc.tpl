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

%for k, op, ninput, exprs in operators:

${op2node[op]}::${op2node[op]}(Node* left, Node* right) : Node(left, right)
{
    %if k == "comparison" or k == "logical":
    vtype(NLS_BUL);
    %elif k == "arithmetic" or k == "bitwise":
    _vtype = left->vtype() >= right->vtype() ? left->vtype() : right->vtype();
    %else:
    FORGOT SOMETHING
    %endif
    stype(EXPR);
}
void ${op2node[op]}::eval(Driver& env)
{
    Node* res = this;
    Node* in1 = left();
    Node* in2 = right();

    VType res_t = res->vtype(); // Evaluate *this
    VType in1_t = in1->vtype();
    VType in2_t = in2->vtype();
    uint64_t mask = (res_t << 32) + (in1_t << 16) + in2_t;
    switch(mask) {
    <%
    expr, sigs = exprs["scalar"]
    %>
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        ${expr.format(res_t=res_vtype, in1_t=in1_vtype, in2_t=in2_vtype)};
        break;
    %endfor

    <%
    expr, sigs = exprs["array"]
    %>
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        res->value().${res_vtype} = new ${vtype2ctype[res_vtype][:-1]}();

        ${expr.format(res_t=res_vtype, in1_t=in1_vtype, in2_t=in2_vtype)};
        res->value().${res_vtype}->setTemp(false);

        if (in1->stype() == EXPR) {
            delete in1->value().${in1_vtype};
            in1->value().${in1_vtype} = NULL;
        }
        if (in2->stype() == EXPR) {
            delete in2->value().${in2_vtype};
            in2->value().${in2_vtype} = NULL;
        }
        break;
    %endfor

    <%
    expr, sigs = exprs["complex"]
    %>
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        res->value().${res_vtype} = new ${vtype2ctype[res_vtype][:-1]}();

        ${expr.format(res_t=res_vtype, in1_t=in1_vtype, in2_t=in2_vtype)};

        break;
    %endfor
    
    default:
        throw std::logic_error(op_vtype_error(res, in1, in2));
        break;
    }

}
string ${op2node[op]}::dot_label(void) { return "${op2node[op]}"; }
%endfor

}

