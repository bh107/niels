#ifndef NIELS_AST_H
#define NIELS_AST_H
#include <cinttypes>
#include <iostream>
#include <sstream>
#include <bxx/bohrium.hpp>
#include <ast_node.hh>
#include <ast_vtype_auto.hh>
#include <ast_unary_ops_auto.hh>
#include <ast_binary_ops_auto.hh>

namespace nls {

class Str : public Node {
public:
    Str(const char* val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class Comment : public Node {
public:
    Comment(const char* comment);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Ident : public Node {
public:
    Ident(const char* val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class Query : public Node {
public:
    Query(Node* left);

    void eval(void);
    std::string dot_label(void);
};

class As : public Node {
public:
    As(Node* left, Node* right);
    void eval(void);
    std::string dot_label(void);
};

class Assign : public Node {
public:
    Assign(Node* left, Node* right);
    std::string dot_label(void);
};

class Alias : public Node {
public:
    Alias(Node* left, Node* right);
    std::string dot_label(void);
};

class Range : public Node {
public:
    Range(bool lb_open, bool ub_open, Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Args : public Node {
public:
    Args(Node* left);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Param : public Node {
public:
    Param(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Params : public Node {
public:
    Params(Node* left);

    std::string dot_label(void);
    std::string dot_shape(void);
};


class Call : public Node {
public:
    Call(Node* left, Node* right);
    std::string dot_label(void);
    std::string dot_shape(void);
};

class Return : public Node {
public:
    Return(Node* left);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Function : public Node {
public:
    Function(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class FunctionDef : public Node {
public:
    FunctionDef(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class FunctionBody : public Node {
public:
    FunctionBody(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Attr : public Node {
public:
    Attr(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class AttrList : public Node {
public:
    AttrList(Node* left);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class RecDef : public Node {
public:
    RecDef(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Shape : public Node {
public:
    Shape(Node* left);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Undefined : public Node {
public:
    Undefined(void);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Empty : public Node {
public:
    Empty(void);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Noop : public Node {
public:
    Noop(void);

    std::string dot_label(void);
    std::string dot_shape(void);
};


class Line : public Node {
public:
    Line(Node* left);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Collection : public Node {
public:
    Collection(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Module : public Node {
public:
    Module(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Block : public Node {
public:
    Block(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class While : public Node {
public:
    While(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class When : public Node {
public:
    When(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class WhenBool : public Node {
public:
    WhenBool(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Cases : public Node {
public:
    Cases(Node* left);
    Cases(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Is : public Node {
public:
    Is(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Otherwise : public Node {
public:
    Otherwise(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Program : public Node {
public:
    Program(void);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class StmtList : public Node {
public:
    StmtList(Node* left);
    StmtList(Node* left, Node* right);

    std::string dot_label(void);
    std::string dot_shape(void);
};

class Import : public Node {
public:
    Import(Node* node);

    std::string dot_label(void);
    std::string dot_shape(void);
};


class Anon : public Node {
public:
    Anon(void);

    std::string dot_label(void);
    std::string dot_shape(void);
};

}
#endif

