#ifndef NLS_AST_EXPR_VTYPE_AUTO_HH
#define NLS_AST_EXPR_VTYPE_AUTO_HH
#include <cinttypes>
#include <string>
#include <nls/ast/node.hh>

namespace nls {
namespace ast {

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

}}

#endif

