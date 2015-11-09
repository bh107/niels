#include <ast.hh>

using namespace std;
namespace nls {

%for vtype, vtype_enum, vtype_ctype, name in vtypes:

// ${name} for ${vtype_ctype} values in union-member ${vtype} enum ${vtype_enum}
${name}::${name}(void) : Node() {
    _value.${vtype} = 0;
    _vtype = ${vtype_enum};
}
${name}::${name}(${vtype_ctype} val) : Node() {
    _value.${vtype} = val;
    _vtype = ${vtype_enum};
}
string ${name}::dot_label(void) { stringstream ss; ss << _value.${vtype}; return ss.str(); }
string ${name}::dot_shape(void) { return "house"; }
string ${name}::dot_color(void) { return "#d9f0d3"; }
%endfor

}

