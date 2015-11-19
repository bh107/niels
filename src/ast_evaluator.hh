#ifndef NIELS_EVALUATOR_HH
#define NIELS_EVALUATOR_HH
#include <ast_visitor.hh>

namespace nls {

class Evaluator : public AstVisitor {
public:

    Evaluator(Driver& driver);
    ~Evaluator(void);
    
    void visit(Node* node);
    void visit(Add* node);

};

}

#endif

