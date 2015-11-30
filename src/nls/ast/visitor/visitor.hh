#ifndef NLS_AST_VISITOR_HH
#define NLS_AST_VISITOR_HH
#include <nls/ast/nodes.hh>

namespace nls {
namespace ast {

// Visitor
class Visitor {
public:
    Visitor(void);
    virtual ~Visitor(void);

    // Generated visit-declarations e.g. "virtual void visit(Add& node) = 0;"
    #include <nls/ast/visitor/visitor_visit_auto_hh.inc>

    /*
        Default iteration, can be used for
        pre or post-order walk within visit(...).
    */
    template<typename T>
    void walk(T& node)
    {
        if (node.left()) {
            node.left()->accept(*this);
        }
        if (node.right()) {
            node.right()->accept(*this);
        }
    }

};

}}

#endif //NLS_AST_VISITOR_HH
