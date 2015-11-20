#ifndef NIELS_EVALUATOR_HH
#define NIELS_EVALUATOR_HH
#include <nls/driver.hh>
#include <nls/ast/visitor.hh>

namespace nls {
namespace ast {

class Evaluator : public Visitor {
public:

    Evaluator(Driver& driver);
    ~Evaluator(void);
    
    void visit(Node* node);
    void visit(Add* node);

};

}}

#endif

