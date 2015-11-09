#ifndef NIELS_AST_BINARY_HH
#define NIELS_AST_BINARY_HH
#include <ast.hh>

namespace nls {

%for name, aname, expr, sigs, ktype in operators:

class ${aname} : public Node {
public:
    ${aname}(Node* in1, Node* in2);
    void eval(void);
    std::string dot_label(void);
};
%endfor

}

#endif

