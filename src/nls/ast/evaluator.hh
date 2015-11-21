#ifndef NIELS_AST_EVALUATOR_HH
#define NIELS_AST_EVALUATOR_HH
#include <nls/ast/visitor.hh>
#include <nls/driver.hh>

namespace nls {
namespace ast {

class Evaluator : public Visitor {
public:

    Evaluator(Driver& driver);
    ~Evaluator(void);
    
    void visit(Node* node);

};

}}

#endif

