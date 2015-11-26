#ifndef NLS_UTILS_HH
#define NLS_UTILS_HH
#include <sstream>
#include <nls/ast/node.hh>
#include <nls/variant.hh>
#include <nls/symbol_table.hh>

namespace nls {

/*
    Return the default string representation as defined
    by the given val's reponse to operator<<
*/
template <typename T>
std::string tos(T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

/*
    Returns a textual description of the value type.

    E.g. i32, bool[], str.
*/
std::string ValueType_text(ValueType value_type);

/*
    Returns a dot-representation of the ast rooted at the given node.

    Note: Implementation uses "embedded" approach to tree-walking,
          concatenating the dot-method result in a pre-order walk.
*/
//std::string dot(ast::Node* node);

}
#endif
