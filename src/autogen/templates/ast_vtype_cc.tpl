#include <ast.hh>

using namespace std;
namespace nls {

%for vtype in vtypes:
<%
ast = vtype2ast[vtype]
enum = vtype2enum[vtype]
ctype = vtype2ctype[vtype]
%>

// ${ast}(Node)
${ast}::${ast}(void) : Node() {
    _value.${vtype} = 0;
    _vtype = ${enum};
}
${ast}::${ast}(${ctype} val) : Node() {
    _value.${vtype} = val;
    _vtype = ${enum};
}
string ${ast}::dot_label(void) { stringstream ss; ss << _value.${vtype}; return ss.str(); }
string ${ast}::dot_shape(void) { return "house"; }
string ${ast}::dot_color(void) { return "#d9f0d3"; }
%endfor

}

