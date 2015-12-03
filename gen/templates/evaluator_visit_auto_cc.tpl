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
%elif node["evaluator"] == "expr" and node["ninput"] == 2:
void Evaluator::visit(${node["class"]}& node)
{
    walk(node);             // Evaluate children

    Variant in2 = pop();
    Variant in1 = pop();    // Pop inputs
    Variant res;            // Allocate result

    uint64_t mask = (in2.value_type << 16) + in1.value_type;
    switch(mask) {
    % for combination in node["code"]:
    <%
    (expr_tpl, typesets) = node["code"][combination] 
    %>
    % for res_value_type, in1_value_type, in2_value_type in typesets:
    <%
    res_enum = "NLS_%s" % res_value_type.upper()
    in1_enum = "NLS_%s" % in1_value_type.upper()
    in2_enum = "NLS_%s" % in2_value_type.upper()
    expr = expr_tpl.format(
        res_t=res_value_type,
        in1_t=in1_value_type,
        in2_t=in2_value_type
    )
    %>
    case (${in2_enum} << 16) + ${in1_enum}:
    % if combination == "aaa":
        res.value.${res_value_type} = new ${res_value_type}_type();
        res.value_type = ${res_enum};
        ${expr};
        delete in1.value.${in1_value_type};
        delete in2.value.${in2_value_type};
        break;
    % elif combination == "aak":
        res.value.${res_value_type} = new ${res_value_type}_type();
        res.value_type = ${res_enum};
        ${expr};
        delete in1.value.${in1_value_type};
    % elif combination == "aka":
        res.value.${res_value_type} = new ${res_value_type}_type();
        res.value_type = ${res_enum};
        ${expr};
        delete in2.value.${in2_value_type};
    % elif combination == "kkk":
        res.value_type = ${res_enum};
        ${expr};
    % endif
        break;
    % endfor
    % endfor
    default:
        cout << "Unsupported types for operator." << endl;
        break;
    }
    
    push(res);              // Push result
}
%elif node["evaluator"] == "expr" and node["ninput"] == 1:
void Evaluator::visit(${node["class"]}& node)
{
    walk(node);             // Evaluate children

    Variant res;            // Allocate result
    Variant in1 = pop();    // Pop input

    uint64_t mask = in1.value_type;
    switch(mask) {
    % for combination in node["code"]:
    <%
    (expr_tpl, typesets) = node["code"][combination] 
    %>
    % for res_value_type, in1_value_type in typesets:
    <%
    res_enum = "NLS_%s" % res_value_type.upper()
    in1_enum = "NLS_%s" % in1_value_type.upper()
    expr = expr_tpl.format(
        res_t=res_value_type,
        in1_t=in1_value_type,
    )
    %>
    case ${in1_enum}:
    % if combination == "aa":
        res.value.${res_value_type} = new ${res_value_type}_type();
        res.value_type = ${res_enum};
        ${expr};
        delete in1.value.${in1_value_type};
        break;
    % elif combination == "ak":
        res.value.${res_value_type} = new ${res_value_type}_type();
        res.value_type = ${res_enum};
        ${expr};
    % elif combination == "kk":
        res.value_type = ${in1_enum};
        ${expr};
    % endif
        break;
    % endfor
    % endfor
    default:
        cout << "Unsupported types for operator." << endl;
        break;
    }
    
    push(res);              // Push result
}
% endif
% endfor

}}

