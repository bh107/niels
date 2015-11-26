#ifndef NLS_AST_NODE_HH
#define NLS_AST_NODE_HH
#include <string>
#include <nls/variant.hh>

namespace nls {
namespace ast {

class Visitor; // Forward declarations

class Node {
public:

    Node(void);

    Node(Node* left);

    Node(Node* left, Node* right);

    ~Node(void);

    virtual void accept(Visitor& visitor) = 0;

    virtual Node* left(void);

    virtual Node* right(void);

    virtual void left(Node* node);

    virtual void right(Node* node);

    virtual Variant* variant(void);

    virtual void variant(Variant* var);

    virtual std::string dot_label(void);

    virtual std::string dot_shape(void);

    virtual std::string dot_color(void);

    void name(std::string& val);
    std::string& name(void);

protected:
    Node* _left;
    Node* _right;

    Variant* _variant;
    std::string _name;
};

}}

#endif //NLS_AST_NODE_HH
