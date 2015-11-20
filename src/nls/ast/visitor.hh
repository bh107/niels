#ifndef NLS_AST_VISITOR_HH
#define NLS_AST_VISITOR_HH
#include <nls/driver.hh>
#include <nls/ast/node.hh>

namespace nls {
namespace ast {

class Visitor {
public:

    Visitor(Driver& driver);
    ~Visitor(void);
    
    virtual void visit(Node* node);

protected:

    Driver& _driver;

};

}}

#endif

