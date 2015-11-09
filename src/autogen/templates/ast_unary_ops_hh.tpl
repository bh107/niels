#ifndef NIELS_AST_UNARY_HH
#define NIELS_AST_UNARY_HH
#include <ast.hh>

namespace nls {

%for name, aname, expr, sigs, ktype in operators:

class ${aname} : public Node {
public:
    ${aname}(Node* in1);
    void eval(void);
    std::string dot_label(void);
};

%endfor

}

#endif

