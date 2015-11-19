#ifndef NIELS_AST_VISITOR_HH
#define NIELS_AST_VISITOR_HH
#include <ast.hh>
#include <driver.hh>

namespace nls {

class AstVisitor {
public:

    AstVisitor(Driver& driver);
    ~AstVisitor(void);
    
    virtual void visit(Node* node);

protected:

    Driver& _driver;

};

}

#endif

