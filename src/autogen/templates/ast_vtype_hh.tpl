#ifndef NIELS_AST_VTYPE_HH
#define NIELS_AST_VTYPE_HH
#include <cinttypes>

namespace nls {

%for vtype, vtype_enum, vtype_ctype, vtype_ast in vtypes:

class ${vtype_ast} : public Node {
public:
    ${vtype_ast}(void);
    ${vtype_ast}(${vtype_ctype} val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};
%endfor

}

#endif

