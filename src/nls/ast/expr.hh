#ifndef NLS_AST_EXPR_HH
#define NLS_AST_EXPR_HH
#include <nls/ast/node.hh>

namespace nls {
namespace ast {

class Comment : public Node {
public:
    Comment(const char* comment);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Ident : public Node {
public:
    Ident(const std::string& name);
    void eval(Driver& env);
    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class Str : public Node {
public:
    Str(const char* val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class As : public Node {
public:
    As(Node* left, Node* right);
    void eval(Driver& env);
    std::string dot_label(void);
};

class Range : public Node {
public:
    Range(bool lb_open, bool ub_open, Node* left, Node* right);

    void eval(Driver& env);
    std::string dot_label(void);
    std::string dot_shape(void);

private:
    bool _lb_open;
    bool _ub_open;
};

class Accessor : public Node {
public:
    Accessor(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Shape : public Node {
public:
    Shape(Node* left);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Call : public Node {
public:
    Call(Node* left, Node* right);
    std::string dot_label(void);
    std::string dot_shape(void);
};

}}

#endif

