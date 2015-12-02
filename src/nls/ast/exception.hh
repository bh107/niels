#ifndef NLS_AST_EXCEPTION_HH
#define NLS_AST_EXCEPTION_HH
#include <exception>

namespace nls {
namespace ast {

class escape_flow : public std::exception {
    virtual const char* what(void) const throw()
    {
        return "Jumping out of control-flow. Somebody catch me.";
    }
};

}}

#endif //NLS_AST_EXCEPTION_HH
