#include <nls/utils.hh>
#include <nls/ast/visitor/evaluator.hh>

using namespace std;
namespace nls {
namespace ast {

%for k, op, ninput, exprs in operators:

void Evaluator::visit(${op2node[op]}& node)
{
    Variant in2 = pop();
    Variant in1 = pop();
    Variant res;

    ValueType res_t = res.value_type;
    ValueType in1_t = in1.value_type;
    ValueType in2_t = in2.value_type;
    uint64_t mask = (res_t << 32) + (in1_t << 16) + in2_t;
    switch(mask) {

    // Konstant = Konstant op Konstant
    <%
    expr, sigs = exprs["kkk"]
    %>
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        ${expr.format(res_t=res_vtype, in1_t=in1_vtype, in2_t=in2_vtype)};
        break;
    %endfor

    // Array = Array op Konstant
    <%
    expr, sigs = exprs["aak"]
    %>
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        res->value.${res_vtype} = new ${vtype2ctype[res_vtype][:-1]}();

        ${expr.format(res_t=res_vtype, in1_t=in1_vtype, in2_t=in2_vtype)};
        res->value.${res_vtype}->setTemp(false);

        /*  TODO: Determine if de-allocation is needed 
        if (typeid(*(node->left())) == EXPR) {
            delete in1.value.${in1_vtype};
            in1.value.${in1_vtype} = NULL;
        }*/
        break;
    %endfor

    // Array = Konstant op Array
    <%
    expr, sigs = exprs["aka"]
    %>
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        res->value.${res_vtype} = new ${vtype2ctype[res_vtype][:-1]}();

        ${expr.format(res_t=res_vtype, in1_t=in1_vtype, in2_t=in2_vtype)};
        res->value.${res_vtype}->setTemp(false);

        /*  TODO: Determine if de-allocation is needed 
        if (typeid(*(node->left()) == EXPR) {
            delete in2->value.${in2_vtype};
            in2->value.${in2_vtype} = NULL;
        }*/
        break;
    %endfor

    // Array = Array op Array
    <%
    expr, sigs = exprs["aaa"]
    %>
    %for res_vtype, in1_vtype, in2_vtype in sigs:
    case (${vtype2enum[res_vtype]} << 32) + (${vtype2enum[in1_vtype]} << 16) + ${vtype2enum[in2_vtype]}:
        res->value.${res_vtype} = new ${vtype2ctype[res_vtype][:-1]}();

        ${expr.format(res_t=res_vtype, in1_t=in1_vtype, in2_t=in2_vtype)};
        res->value.${res_vtype}->setTemp(false);

        /*  TODO: Determine if de-allocation is needed 
        if (in1->stype() == EXPR) {
            delete in1->value.${in1_vtype};
            in1->value.${in1_vtype} = NULL;
        }
        if (in2->stype() == EXPR) {
            delete in2->value.${in2_vtype};
            in2->value.${in2_vtype} = NULL;
        }*/
        break;
    %endfor
    
    default:
        throw std::logic_error(value_type_error(res, in1, in2));
        break;
    }

    push(res);
}

%endfor

}}

