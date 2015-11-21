#ifndef NLS_AST_EXPR_UNARY_AUTO_HH
#define NLS_AST_EXPR_UNARY_AUTO_HH
#include <nls/ast/node.hh>
#include <nls/driver.hh>

namespace nls {
namespace ast {

%for k, op, ninput, exprs in operators:

class ${op2node[op]} : public Node {
public:
    ${op2node[op]}(Node* in1);
    void eval(Driver& env);
    std::string dot_label(void);
};

%endfor

}}

#endif
