#ifndef NLS_UTILS_HH
#define NLS_UTILS_HH
#include <sstream>
#include <nls/value.hh>
#include <nls/symboltable.hh>

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
    Returns a textual description of the structural type.

    E.g. variable, record, "record definition".
*/
std::string SType_text(SType stype);

/*
    Returns a textual description of the value type.

    E.g. i32, bool[], str.
*/
std::string VType_text(VType vtype);

/*
    Returns a dot-representation of the ast rooted at the given node.

    Note: Implementation uses "embedded" approach to tree-walking,
          concatenating the dot-method result in a pre-order walk.
*/
std::string dot(Node* node);

}
#endif

