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


    Node* left(void);

    Node* right(void);

    void left(Node* node);

    void right(Node* node);

    void append(Node* node);

    Variant* variant(void);

    void variant(Variant* var);

    void name(std::string& val);

    std::string& name(void);

    virtual void accept(Visitor& visitor) = 0;

    virtual std::string dot_label(void);

    virtual std::string dot_shape(void);

    virtual std::string dot_color(void);

protected:
    Node* _left;
    Node* _right;

    Variant* _variant;
    std::string _name;
};

}}

#endif //NLS_AST_NODE_HH
