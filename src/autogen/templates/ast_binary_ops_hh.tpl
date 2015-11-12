#ifndef NIELS_AST_BINARY_HH
#define NIELS_AST_BINARY_HH
#include <ast.hh>

namespace nls {

%for k, op, ninput, exprs in operators:

class ${op2node[op]} : public Node {
public:
    ${op2node[op]}(Node* in1, Node* in2);
    void eval(Driver& env);
    std::string dot_label(void);
};
%endfor

}

#endif

