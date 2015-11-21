#ifndef NLS_AST_NODE_HH
#define NLS_AST_NODE_HH
#include <string>
#include <nls/value.hh>

namespace nls {
class Driver;   // Forward declaration

namespace ast {

class Node {
public:
    Node(void);
    Node(Node* left);
    Node(Node* left, Node* right);

    virtual ~Node(void);

    Node* left(void);
    Node* right(void);
    void left(Node* left);
    void right(Node* right);

    void append(Node* node);

    VType vtype(void);
    void vtype(VType val);

    SType stype(void);
    void stype(SType val);

    void value(Value& val);

    Value& value(void);

    bool defined(void);
    bool known(void);

    std::string& str(void);

    std::string dot(void);
    std::string dot_relation(void);
    virtual std::string txt(void);

    std::string& name(void);
    void name(const std::string& val);

    virtual void eval(Driver& env);

    virtual std::string dot_shape(void);
    virtual std::string dot_label(void);
    virtual std::string dot_color(void);
    
    template <typename T>
    void visit(T& visitor);

protected:
    SType _stype;
    VType _vtype;
    Value _value;

    Node* _left;
    Node* _right;

    std::string _name;
};

}}

#endif

