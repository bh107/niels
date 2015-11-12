#ifndef NIELS_AST_VTYPE_HH
#define NIELS_AST_VTYPE_HH
#include <cinttypes>

namespace nls {

%for vtype in vtypes:
<%
ast = vtype2ast[vtype]
enum = vtype2enum[vtype]
ctype = vtype2ctype[vtype]
%>

class ${ast} : public Node {
public:
    ${ast}(void);
    ${ast}(${ctype} val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};
%endfor

}

#endif

