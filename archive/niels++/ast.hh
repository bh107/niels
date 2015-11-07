#ifndef NIELS_AST_H
#define NIELS_AST_H
#include <cinttypes>
#include <cstddef>
#include <sstream>
#include <iostream>

namespace nls {

union Value {
    int32_t int32;
    int64_t int64;
    float real32;
    double real64;
    bool bul;
    std::string* str;
};
typedef union Value Value;

enum VType {
    S_BOOL,
    S_INT32,
    S_INT64,
    S_REAL32,
    S_REAL64,
    S_STR,

    A_BOOL,
    A_INT32,
    A_INT64,
    A_REAL32,
    A_REAL64,

    UNDEFINED
};
typedef enum VType VType;

std::string VType_text(VType vtype);

enum SType {
    VAR,
    FUNC,
    MOD,
    COLL,

    UNKNOWN
};
typedef enum SType SType;

std::string SType_text(SType stype);

class Node {
public:
    Node(void);
    Node(Node* left);
    Node(Node* left, Node* right);

    Node(const Node& node);
    Node(Node& left);
    Node(Node& left, Node& right);

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
    std::string txt(void);

    virtual std::string dot_shape(void);
    virtual std::string dot_label(void);
    virtual std::string dot_color(void);
    
    virtual ~Node(void);

protected:
    SType _stype;
    VType _vtype;
    Value _value;

    Node* _left;
    Node* _right;
};

//
// Literals
//

class Int32 : public Node {
public:
    Int32(void);
    Int32(int32_t val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class Int64 : public Node {
public:
    Int64(void);
    Int64(int64_t val);
    Int64(Node& node);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class Real32 : public Node {
public:
    Real32(void);
    Real32(float val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class Real64 : public Node {
public:
    Real64(void);
    Real64(double val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

class Bool : public Node {
public:
    Bool(void);
    Bool(bool val);

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

class Comment : public Node {
public:
    Comment(const char* comment);

    std::string dot_label(void);
    std::string dot_shape(void);
};

//
// Identifiers
//
class Ident : public Node {
public:
    Ident(const char* val);
    Ident(std::string& val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

//
// Unary Arithmetic Operators
//
class Neg : public Node {
public:
    Neg(Node* left);
    std::string dot_label(void);
};

class Inv : public Node {
public:
    Inv(Node* left);
    std::string dot_label(void);
};

//
// Query
//
class Query : public Node {
public:
    Query(Node* left);
    std::string dot_label(void);
};

//
// Binary Arithmetic Operators
//
class Add : public Node {
public:
    Add(Node* left, Node* right);
    Add(Node& left, Node& right);
    std::string dot_label(void);
};

class Sub : public Node {
public:
    Sub(Node* left, Node* right);
    std::string dot_label(void);
};

class Mul : public Node {
public:
    Mul(Node* left, Node* right);
    std::string dot_label(void);
};

class Mod : public Node {
public:
    Mod(Node* left, Node* right);
    std::string dot_label(void);
};

class Div : public Node {
public:
    Div(Node* left, Node* right);
    std::string dot_label(void);
};

class Pow : public Node {
public:
    Pow(Node* left, Node* right);
    std::string dot_label(void);
};

class LThan : public Node {
public:
    LThan(Node* left, Node* right);
    std::string dot_label(void);
};

class As : public Node {
public:
    As(Node* left, Node* right);
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

