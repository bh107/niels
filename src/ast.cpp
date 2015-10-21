#include "ast.h"
#include <typeinfo>

using namespace std;

namespace nir {

string VType_text(VType vtype)
{
    switch(vtype) {
        case UNDEFINED: return "undef";
        case S_STR:     return "str";
        
        case S_INT32:   return "i32";
        case S_INT64:   return "i64";
        case S_REAL32:  return "r32";
        case S_REAL64:  return "r64";
        case S_BOOL:    return "bool";

        case A_INT32:   return "i32[]";
        case A_INT64:   return "i64[]";
        case A_REAL32:  return "r32[]";
        case A_REAL64:  return "r64[]";
        case A_BOOL:    return "bool[]";
    }
}

//
//  Node
//
Node::Node(void)                    : _vtype(UNDEFINED), _left(NULL), _right(NULL) {}
Node::Node(Node* left)              : _vtype(left->vtype()), _left(left), _right(NULL) {
    _vtype = left->vtype();
}
Node::Node(Node* left, Node* right) : _vtype(UNDEFINED), _left(left), _right(right) {
    _vtype = left->vtype() >= right->vtype() ? left->vtype() : right->vtype();
}

void Node::left(Node* left) { _left = left; }
void Node::right(Node* right) { _right = right; }

Node* Node::left(void) { return _left; }
Node* Node::right(void) { return _right; }

string Node::dot(void)
{
    stringstream ss;
    ss  << "N" << this << "["
        << "style=filled" << ","
        << "label=" << dot_label() << ","
        << "shape=" << dot_shape() << ","
        << "fillcolor=\"" << dot_color() << "\""
        << "]" << endl;

    ss << dot_relation();   
    return ss.str();
}

void Node::vtype(VType vtype)
{
    _vtype = vtype;
}

VType Node::vtype(void)
{
    return _vtype;
}

string Node::dot_relation(void)
{
    stringstream ss;
    if (_left) {
        ss << "N" << this << " -- N" << _left
           << "[label=\"" << VType_text(_left->vtype()) << "\""
           << "]" << endl;
    }
    if (_right) {
        ss << "N" << this << " -- N" << _right
           << "[label=\"" << VType_text(_right->vtype())  << "\""
           << "]" << endl;

        if (typeid(*this) == typeid(StmtList)) {
            ss << "{rank=same; " << "N" << this << " N" << _right << " }" << endl;
        }
        /*
        if (((typeid(*this) == typeid(Line)) || (typeid(*this) == typeid(EmptyLine))) && \
            ((typeid(*_right) == typeid(Line)) || (typeid(*_right) == typeid(EmptyLine)))) {
            ss << "{rank=same; " << "N" << this << " N" << _right << " }" << endl;
        }
        */
    }
    return ss.str();
}

string Node::dot_shape(void) { return "box"; }
string Node::dot_label(void) { return "b0rk3d"; }
string Node::dot_color(void) { return "#e0e0e0"; }

//
// Literals
//
Int32::Int32(int32_t val) : Node() {
    _value.int32 = val;
    _vtype = S_INT32;
}
string Int32::dot_label(void) { stringstream ss; ss << _value.int32; return ss.str(); }
string Int32::dot_shape(void) { return "house"; }
string Int32::dot_color(void) { return "#d9f0d3"; }

Int64::Int64(int64_t val) : Node() {
    _value.int64 = val;
    _vtype = S_INT64;
}
string Int64::dot_label(void) { stringstream ss; ss << _value.int64; return ss.str(); }
string Int64::dot_shape(void) { return "house"; }
string Int64::dot_color(void) { return "#d9f0d3"; }

Real32::Real32(float val) : Node() {
    _value.real32 = val;
    _vtype = S_REAL32;
}
string Real32::dot_label(void) { stringstream ss; ss << _value.real32; return ss.str(); }
string Real32::dot_shape(void) { return "house"; }
string Real32::dot_color(void) { return "#d9f0d3"; }

Real64::Real64(double val) : Node() {
    _value.real64 = val;
    _vtype = S_REAL64;
}
string Real64::dot_label(void) { stringstream ss; ss << _value.real64; return ss.str(); }
string Real64::dot_shape(void) { return "house"; }
string Real64::dot_color(void) { return "#d9f0d3"; }

Bool::Bool(bool val) : Node() {
    _value.boolean = val;
    _vtype = S_BOOL;
}
string Bool::dot_label(void) { stringstream ss; ss << _value.boolean; return ss.str(); }
string Bool::dot_shape(void) { return "house"; }
string Bool::dot_color(void) { return "#d9f0d3"; }

Str::Str(char* val) : Node() {
    _value.str = val;
    _vtype = S_STR;
}
string Str::dot_label(void) { stringstream ss; ss << _value.str; return ss.str(); }
string Str::dot_shape(void) { return "house"; }
string Str::dot_color(void) { return "#5aae61"; }

Ident::Ident(char* name) : Node()
{
    _value.str = name;
}
string Ident::dot_label(void) { stringstream ss; ss << _value.str; return ss.str(); }
string Ident::dot_shape(void) { return "hexagon"; }
string Ident::dot_color(void) { return "#fee0b6"; }

Query::Query(Node* left) : Node(left) {}
string Query::dot_label(void) { return "Query"; }

Neg::Neg(Node* left) : Node(left) {}
string Neg::dot_label(void) { return "Neg"; }

Inv::Inv(Node* left) : Node(left) {}
string Inv::dot_label(void) { return "Inv"; }

Add::Add(Node* left, Node* right) : Node(left, right) {}
string Add::dot_label(void) { return "Add"; }

Sub::Sub(Node* left, Node* right) : Node(left, right) {}
string Sub::dot_label(void) { return "Sub"; }

Mul::Mul(Node* left, Node* right) : Node(left, right) {}
string Mul::dot_label(void) { return "Mul"; }

Mod::Mod(Node* left, Node* right) : Node(left, right) {}
string Mod::dot_label(void) { return "Mod"; }

Pow::Pow(Node* left, Node* right) : Node(left, right) {}
string Pow::dot_label(void) { return "Pow"; }

Div::Div(Node* left, Node* right) : Node(left, right) {}
string Div::dot_label(void) { return "Div"; }

LThan::LThan(Node* left, Node* right) : Node(left, right) {}
string LThan::dot_label(void) { return "LThan"; }

As::As(Node* left, Node* right) : Node(left, right) {}
string As::dot_label(void) { return "As"; }

Assign::Assign(Node* left, Node* right) : Node(left, right) {
    left->vtype(right->vtype());    // Derive the type
    vtype(right->vtype());
}
string Assign::dot_label(void) { return "Assign"; }

Alias::Alias(Node* left, Node* right) : Node(left, right) {
    left->vtype(right->vtype());
    vtype(right->vtype());
}
string Alias::dot_label(void) { return "Alias"; }

Range::Range(bool lb_open, bool ub_open, Node* left, Node* right) : Node(left, right) {}
string Range::dot_label(void) { return "Range"; }
string Range::dot_shape(void) { return "trapezium"; }

Shape::Shape(Node* left) : Node(left) {}
string Shape::dot_label(void) { return "Shape"; }
string Shape::dot_shape(void) { return "trapezium"; }

Arg::Arg(Node* left) : Node(left) {}
string Arg::dot_label(void) { return "Arg"; }
string Arg::dot_shape(void) { return "diamond"; }

Param::Param(Node* left) : Node(left) {}
string Param::dot_label(void) { return "Param"; }
string Param::dot_shape(void) { return "diamond"; }

FuncCall::FuncCall(Node* left, Node* right) : Node(left, right) {}
string FuncCall::dot_label(void) { return "FuncCall"; }
string FuncCall::dot_shape(void) { return "parallelogram"; }

FuncDef::FuncDef(Node* left, Node* right) : Node(left, right) {}
string FuncDef::dot_label(void) { return "FuncDef"; }
string FuncDef::dot_shape(void) { return "parallelogram"; }

Block::Block(Node* left, Node* right) : Node(left, right) { }
string Block::dot_label(void) { return "Block"; }
string Block::dot_shape(void) { return "box"; }

While::While(Node* left, Node* right) : Node(left, right) { }
string While::dot_label(void) { return "While"; }
string While::dot_shape(void) { return "box"; }

When::When(Node* left, Node* right) : Node(left, right) { }
string When::dot_label(void) { return "When"; }
string When::dot_shape(void) { return "box"; }

CaseList::CaseList() : Node() { }
CaseList::CaseList(Node* left) : Node(left) { }
string CaseList::dot_label(void) { return "CaseList"; }
string CaseList::dot_shape(void) { return "box"; }

Is::Is(Node* left, Node* right) : Node(left, right) { }
string Is::dot_label(void) { return "Is"; }
string Is::dot_shape(void) { return "box"; }

Otherwise::Otherwise(Node* left, Node* right) : Node(left, right) { }
string Otherwise::dot_label(void) { return "Otherwise"; }
string Otherwise::dot_shape(void) { return "box"; }


EmptyLine::EmptyLine(void) : Node() {}
string EmptyLine::dot_label(void) { return "EmptyLine"; }
string EmptyLine::dot_shape(void) { return "box"; }

Noop::Noop(void) : Node() {}
string Noop::dot_label(void) { return "Noop"; }
string Noop::dot_shape(void) { return "box"; }

Line::Line(Node* left) : Node(left) {}
string Line::dot_label(void) { return "Line"; }
string Line::dot_shape(void) { return "box"; }

StmtList::StmtList(Node* left) : Node(left) {}
StmtList::StmtList(Node* left, Node* right) : Node(left, right) {}
string StmtList::dot_label(void) { return "StmtList"; }
string StmtList::dot_shape(void) { return "box"; }

Comment::Comment(char* comment) : Node() {
    _value.str = comment;
    _vtype = S_STR;
}
string Comment::dot_label(void) { return "Comment"; }
string Comment::dot_shape(void) { return "box"; }

//
// Pretty Printing
//
string dot(Node* node) {
    if (node) {
        if (node->left() and node->right()) {
            return node->dot() + dot(node->left()) + dot(node->right());
        } else if (node->left() and ! node->right()) {
            return node->dot() + dot(node->left());
        } else if (!node->left() and node->right()) {
            return node->dot() + dot(node->right());
        } else {
            return node->dot();
        }
    }    
}

Program::Program(void) : Node() {}
string Program::dot_label(void) { return "Program"; }
string Program::dot_shape(void) { return "box"; }

Anon::Anon(void) : Node() {}
string Anon::dot_label(void) { return "Anon"; }
string Anon::dot_shape(void) { return "box"; }

void eval(Node* node) { }

}

