#ifndef NLS_AST_NODE_HH
#define NLS_AST_NODE_HH
#include <string>
#include <nls/ast/visitor.hh>

namespace nls {
namespace ast {

class Node {
public:
    Node(void);

    virtual ~Node(void);
    
    template <typename T>
    void visit(T& visitor);

    std::string dot(void);
    std::string dot_relation(void);
    virtual std::string txt(void);

protected:

};

}}

#endif

