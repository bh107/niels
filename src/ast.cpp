#include "ast.h"
#include "symboltable.h"
#include <typeinfo>
#include <iomanip>
#include <exception>

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

string SType_text(SType stype)
{
    switch(stype) {
        case VAR:       return "var";
        case FUNC:      return "function";
        case MOD:       return "module";
        case COLL:      return "collection";
        case UNKNOWN:   return "unknown";
    }
}

//
//  Node
//
Node::Node(void)                    : _stype(UNKNOWN), _vtype(UNDEFINED), _left(NULL), _right(NULL) {}
Node::Node(Node* left)              : _stype(UNKNOWN), _vtype(left->vtype()), _left(left), _right(NULL) {
}
Node::Node(Node* left, Node* right) : _stype(UNKNOWN), _vtype(UNDEFINED), _left(left), _right(right) {
    _vtype = left->vtype() >= right->vtype() ? left->vtype() : right->vtype();
}

Node* Node::left(void) { return _left; }
Node* Node::right(void) { return _right; }

void Node::left(Node* left) { _left = left; }
void Node::right(Node* right) { _right = right; }

void Node::append(Node* node) {
    Node* next = this;
    while(next->right()) {
        next = next->right();
    }
    next->right(node);
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

        if ((typeid(*this) == typeid(StmtList)) or \
            (typeid(*this) == typeid(Cases))) {
            ss << "{rank=same; " << "N" << this << " N" << _right << " }" << endl;
        }
    }
    return ss.str();
}

string Node::dot(void)
{
    stringstream ss;
    ss << "N" << this << "[";
    ss << "style=filled" << ",";
    ss << "label=" << dot_label() << ",";
    ss << "shape=" << dot_shape() << ",";
    ss << "fillcolor=\"" << dot_color() << "\"";
    ss << "]" << endl;

    return ss.str();
}

string Node::txt(void)
{
    stringstream ss;
    ss << SType_text(stype()) << " of " << VType_text(vtype());

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

string& Node::str(void)
{
    return *(_value.str);
}

Value& Node::value(void)
{
    return _value;
}

void Node::value(Value& val)
{
    _value = val;
}

void Node::stype(SType val)
{
    _stype = val;
}

SType Node::stype(void)
{
    return _stype;
}

bool Node::defined(void)
{
    return _vtype != UNDEFINED;
}

bool Node::known(void)
{
    return _stype != UNKNOWN;
}

Node::~Node(void) {
    if (vtype() == S_STR) {
        delete _value.str;
    }
}
string Node::dot_shape(void) { return "box"; }
string Node::dot_label(void) { return "b0rk3d"; }
string Node::dot_color(void) { return "#e0e0e0"; }

//
// Literals
//
Int32::Int32(void) : Node() {
    _value.int32 = 0;
    _vtype = S_INT32;
}
Int32::Int32(int32_t val) : Node() {
    _value.int32 = val;
    _vtype = S_INT32;
}
string Int32::dot_label(void) { stringstream ss; ss << _value.int32; return ss.str(); }
string Int32::dot_shape(void) { return "house"; }
string Int32::dot_color(void) { return "#d9f0d3"; }

Int64::Int64(void) : Node() {
    _value.int64 = 0;
    _vtype = S_INT64;
}
Int64::Int64(int64_t val) : Node() {
    _value.int64 = val;
    _vtype = S_INT64;
}
string Int64::dot_label(void) { stringstream ss; ss << _value.int64; return ss.str(); }
string Int64::dot_shape(void) { return "house"; }
string Int64::dot_color(void) { return "#d9f0d3"; }

Real32::Real32(void) : Node() {
    _value.real32 = 0.0;
    _vtype = S_REAL32;
}
Real32::Real32(float val) : Node() {
    _value.real32 = val;
    _vtype = S_REAL32;
}
string Real32::dot_label(void) { stringstream ss; ss << fixed << setprecision(2) << _value.real32; return ss.str(); }
string Real32::dot_shape(void) { return "house"; }
string Real32::dot_color(void) { return "#d9f0d3"; }

Real64::Real64(void) : Node() {
    _value.real64 = 0.0;
    _vtype = S_REAL64;
}
Real64::Real64(double val) : Node() {
    _value.real64 = val;
    _vtype = S_REAL64;
}
string Real64::dot_label(void) { stringstream ss; ss << fixed << setprecision(2) << _value.real64; return ss.str(); }
string Real64::dot_shape(void) { return "house"; }
string Real64::dot_color(void) { return "#d9f0d3"; }

Bool::Bool(void) : Node() {
    _value.boolean = false;
    _vtype = S_BOOL;
}
Bool::Bool(bool val) : Node() {
    _value.boolean = val;
    _vtype = S_BOOL;
}
string Bool::dot_label(void) { stringstream ss; ss << boolalpha << _value.boolean; return ss.str(); }
string Bool::dot_shape(void) { return "house"; }
string Bool::dot_color(void) { return "#d9f0d3"; }

Comment::Comment(const char* comment) : Node() {
    _value.str = new string(comment);
    _vtype = S_STR;
}
string Comment::dot_label(void) { return "Comment"; }
string Comment::dot_shape(void) { return "box"; }

Str::Str(const char* val) : Node() {
    _value.str = new string(val);
    _vtype = S_STR;
}
string Str::dot_label(void) { return *_value.str; }
string Str::dot_shape(void) { return "house"; }
string Str::dot_color(void) { return "#5aae61"; }

Ident::Ident(const char* name) : Node()
{
    _value.str = new string(name);
}
string Ident::dot_label(void) { return *_value.str; }
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

As::As(Node* left, Node* right) : Node(left, right) {
    switch(left->vtype()) {
        case S_BOOL:    vtype(A_BOOL);  break;
        case S_INT32:   vtype(A_INT32); break;
        case S_INT64:   vtype(A_INT64); break;
        case S_REAL32:  vtype(A_REAL32); break;
        case S_REAL64:  vtype(A_REAL64); break;
        case S_STR:     throw logic_error("Array of strings is unsupported.");
        case A_BOOL:    throw logic_error("Array of arrays is unsupported.");
        case A_INT32:   throw logic_error("Array of arrays is unsupported.");
        case A_INT64:   throw logic_error("Array of arrays is unsupported.");
        case A_REAL32:  throw logic_error("Array of arrays is unsupported.");
        case A_REAL64:  throw logic_error("Array of arrays is unsupported.");
        case UNDEFINED: throw logic_error("Cannot construct array of undefined.");
    }
}
string As::dot_label(void) { return "As"; }

Assign::Assign(Node* left, Node* right) : Node(left, right) {

    if (left->defined() && (left->vtype() != right->vtype())) {
        stringstream ss;
        ss << "Assigning " << right->txt();
        ss << " to " << left->str() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (left->known() && (left->stype() != VAR)) {
        stringstream ss;
        ss << "Assigning " << right->txt();
        ss << " to " << left->str() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (!left->defined()) {   // Derive the type of IDENT from right
        left->vtype(right->vtype());    
        left->stype(VAR);
    }
    vtype(left->vtype());               // IDENT determines type
}
string Assign::dot_label(void) { return "Assign"; }

Alias::Alias(Node* left, Node* right) : Node(left, right) {
    if ((left->vtype() != UNDEFINED) && (left->vtype() != right->vtype())) {
        stringstream ss;
        ss << "Aliasing " << right->txt();
        ss << " to " << left->str() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if ((left->stype() != UNKNOWN) && (left->stype() != VAR)) {
        stringstream ss;
        ss << "Aliasing " << right->txt();
        ss << " to " << left->str() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (left->vtype() == UNDEFINED) {   // Derive the type of IDENT from right
        left->vtype(right->vtype());    
        left->stype(VAR);
    }
    vtype(left->vtype());               // IDENT determines type
}
string Alias::dot_label(void) { return "Alias"; }

Range::Range(bool lb_open, bool ub_open, Node* left, Node* right) : Node(left, right) {}
string Range::dot_label(void) { return "Range"; }
string Range::dot_shape(void) { return "trapezium"; }

Shape::Shape(Node* left) : Node(left) {}
string Shape::dot_label(void) { return "Shape"; }
string Shape::dot_shape(void) { return "trapezium"; }

Args::Args(Node* left) : Node(left) {}
string Args::dot_label(void) { return "Args"; }
string Args::dot_shape(void) { return "diamond"; }

Param::Param(Node* left, Node* right) : Node(left, right) {
    if (left->vtype() == UNDEFINED) {   // Derive the type
        left->vtype(right->vtype());    
    }
    vtype(right->vtype());
}
string Param::dot_label(void) { return "Param"; }
string Param::dot_shape(void) { return "diamond"; }

Params::Params(Node* left) : Node(left) {}
string Params::dot_label(void) { return "Params"; }
string Params::dot_shape(void) { return "diamond"; }

Call::Call(Node* left, Node* right) : Node(left, right) {}
string Call::dot_label(void) { return "Call"; }
string Call::dot_shape(void) { return "parallelogram"; }

Return::Return(Node* left) : Node(left) {}
string Return::dot_label(void) { return "Return"; }
string Return::dot_shape(void) { return "box"; }

Function::Function(Node* left, Node* right) : Node(left, right) {
    _stype = FUNC;
}
string Function::dot_label(void) { return "Function"; }
string Function::dot_shape(void) { return "parallelogram"; }

FunctionDef::FunctionDef(Node* left, Node* right) : Node(left, right) {}
string FunctionDef::dot_label(void) { return "FunctionDef"; }
string FunctionDef::dot_shape(void) { return "parallelogram"; }

FunctionBody::FunctionBody(Node* left, Node* right) : Node(left, right) {}
string FunctionBody::dot_label(void) { return "FunctionBody"; }
string FunctionBody::dot_shape(void) { return "parallelogram"; }

Attr::Attr(Node* left, Node* right) : Node(left, right) {}
string Attr::dot_label(void) { return "Attr"; }
string Attr::dot_shape(void) { return "parallelogram"; }

AttrList::AttrList(Node* left) : Node(left) {}
string AttrList::dot_label(void) { return "AttrList"; }
string AttrList::dot_shape(void) { return "parallelogram"; }

RecDef::RecDef(Node* left, Node* right) : Node(left, right) {}
string RecDef::dot_label(void) { return "RecDef"; }
string RecDef::dot_shape(void) { return "parallelogram"; }

Collection::Collection(Node* left, Node* right) : Node(left, right) { }
string Collection::dot_label(void) { return "Collection"; }
string Collection::dot_shape(void) { return "box"; }

Module::Module(Node* left, Node* right) : Node(left, right) { }
string Module::dot_label(void) { return "Module"; }
string Module::dot_shape(void) { return "box"; }

Block::Block(Node* left, Node* right) : Node(left) {
    if (typeid(*right) != typeid(StmtList)) {
        nir::Node* stmtList = new nir::StmtList(right);
        stmtList->left(NULL);
        stmtList->right(right);
        this->right(stmtList);
    } else {
        this->right(right);
    }
}
string Block::dot_label(void) { return "Block"; }
string Block::dot_shape(void) { return "box"; }

While::While(Node* left, Node* right) : Node(left, right) { }
string While::dot_label(void) { return "While"; }
string While::dot_shape(void) { return "box"; }

When::When(Node* left, Node* right) : Node(left, right) { }
string When::dot_label(void) { return "When"; }
string When::dot_shape(void) { return "box"; }

WhenBool::WhenBool(Node* left, Node* right) : Node(left, right) { }
string WhenBool::dot_label(void) { return "WhenBool"; }
string WhenBool::dot_shape(void) { return "box"; }

Cases::Cases(Node* left) : Node(left) {}
Cases::Cases(Node* left, Node* right) : Node(left, right) {}
string Cases::dot_label(void) { return "Cases"; }
string Cases::dot_shape(void) { return "box"; }

Is::Is(Node* left, Node* right) : Node(left, right) { }
string Is::dot_label(void) { return "Is"; }
string Is::dot_shape(void) { return "box"; }

Otherwise::Otherwise(Node* left, Node* right) : Node(left, right) { }
string Otherwise::dot_label(void) { return "Otherwise"; }
string Otherwise::dot_shape(void) { return "box"; }

Undefined::Undefined(void) : Node() {}
string Undefined::dot_label(void) { return "Undefined"; }
string Undefined::dot_shape(void) { return "box"; }

Empty::Empty(void) : Node() {}
string Empty::dot_label(void) { return "Empty"; }
string Empty::dot_shape(void) { return "box"; }

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


Import::Import(Node* left) : Node(left) {}
string Import::dot_label(void) { return "Import"; }
string Import::dot_shape(void) { return "box"; }

Program::Program(void) : Node() {}
string Program::dot_label(void) { return "Program"; }
string Program::dot_shape(void) { return "box"; }

Anon::Anon(void) : Node() {}
string Anon::dot_label(void) { return "Anon"; }
string Anon::dot_shape(void) { return "box"; }

}
