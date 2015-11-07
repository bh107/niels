#include <typeinfo>
#include <exception>
#include <iomanip>
#include "ast.hh"
#include "symboltable.hh"

using namespace std;

namespace nls {

#include "scalar_ops.cc"

string VType_text(VType vtype)
{
    switch(vtype) {
        case UNDEFINED: return "undef";
        case STR:     return "str";
        
        case I32:   return "i32";
        case I64:   return "i64";
        case R32:  return "r32";
        case R64:  return "r64";
        case BOOL:    return "bool";

        case I32_A:   return "i32[]";
        case I64_A:   return "i64[]";
        case R32_A:  return "r32[]";
        case R64_A:  return "r64[]";
        case BOOL_A:    return "bool[]";
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
Node::Node(void)
    : _stype(UNKNOWN), _vtype(UNDEFINED), _left(NULL), _right(NULL)
{
}
Node::Node(Node* left)
    : _stype(UNKNOWN), _vtype(left->vtype()), _left(left), _right(NULL)
{
}
Node::Node(Node* left, Node* right) : _stype(UNKNOWN), _vtype(UNDEFINED), _left(left), _right(right)
{
    _vtype = left->vtype() >= right->vtype() ? left->vtype() : right->vtype();
}

Node* Node::left(void)
{
    return _left;
}
Node* Node::right(void) {
    return _right;
}

void Node::left(Node* left)
{
    _left = left;
}
void Node::right(Node* right)
{
    _right = right;
}

void Node::append(Node* node)
{
    Node* next = this;
    while(next->right()) {
        next = next->right();
    }
    next->right(node);
}

void Node::eval(void)
{

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
    if (vtype() == STR) {
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
    _value.i32 = 0;
    _vtype = I32;
}
Int32::Int32(int32_t val) : Node() {
    _value.i32 = val;
    _vtype = I32;
}
string Int32::dot_label(void) { stringstream ss; ss << _value.i32; return ss.str(); }
string Int32::dot_shape(void) { return "house"; }
string Int32::dot_color(void) { return "#d9f0d3"; }

Int64::Int64(void) : Node() {
    _value.i64 = 0;
    _vtype = I64;
}
Int64::Int64(int64_t val) : Node() {
    _value.i64 = val;
    _vtype = I64;
}
string Int64::dot_label(void) { stringstream ss; ss << _value.i64; return ss.str(); }
string Int64::dot_shape(void) { return "house"; }
string Int64::dot_color(void) { return "#d9f0d3"; }

Real32::Real32(void) : Node() {
    _value.r32 = 0.0;
    _vtype = R32;
}
Real32::Real32(float val) : Node() {
    _value.r32 = val;
    _vtype = R32;
}
string Real32::dot_label(void) { stringstream ss; ss << fixed << setprecision(2) << _value.r32; return ss.str(); }
string Real32::dot_shape(void) { return "house"; }
string Real32::dot_color(void) { return "#d9f0d3"; }

Real64::Real64(void) : Node() {
    _value.r64 = 0.0;
    _vtype = R64;
}
Real64::Real64(double val) : Node() {
    _value.r64 = val;
    _vtype = R64;
}
string Real64::dot_label(void) { stringstream ss; ss << fixed << setprecision(2) << _value.r64; return ss.str(); }
string Real64::dot_shape(void) { return "house"; }
string Real64::dot_color(void) { return "#d9f0d3"; }

Bool::Bool(void) : Node() {
    _value.boolean = false;
    _vtype = BOOL;
}
Bool::Bool(bool val) : Node() {
    _value.boolean = val;
    _vtype = BOOL;
}
string Bool::dot_label(void) { stringstream ss; ss << boolalpha << _value.boolean; return ss.str(); }
string Bool::dot_shape(void) { return "house"; }
string Bool::dot_color(void) { return "#d9f0d3"; }

Comment::Comment(const char* comment) : Node() {
    _value.str = new string(comment);
    _vtype = STR;
}
string Comment::dot_label(void) { return "Comment"; }
string Comment::dot_shape(void) { return "box"; }

Str::Str(const char* val) : Node() {
    _value.str = new string(val);
    _vtype = STR;
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
void Query::eval(void)
{
    left()->eval();

    switch(left()->vtype()) {
    case UNDEFINED:
        cout << "I have no idea what it is..." << endl;
    case STR:
        cout << *(left()->value().str) << endl;
        break;
    case I32:
        cout << left()->value().i32 << endl;
        break;
    case I64:
        cout << left()->value().i64 << endl;
        break;
    case R32:
        cout << left()->value().r32 << endl;
        break;
    case R64:
        cout << left()->value().r64 << endl;
        break;
    case BOOL:
        cout << boolalpha << left()->value().boolean << endl;
        break;

    case I32_A:
        cout << *((bxx::multi_array<int32_t>*)(left()->value().array)) << endl;
        break;
    case I64_A:
        cout << *((bxx::multi_array<int64_t>*)(left()->value().array)) << endl;
        break;
    case R32_A:
        cout << *((bxx::multi_array<float>*)(left()->value().array)) << endl;
        break;
    case R64_A:
        cout << *((bxx::multi_array<double>*)(left()->value().array)) << endl;
        break;
    case BOOL_A:
        cout << *((bxx::multi_array<bool>*)(left()->value().array)) << endl;
        break;
    }

    if (typeid(*left()) == typeid(As)) { // TODO: Find a better way to clean up...
        switch(left()->vtype()) {
        case I32_A:
            delete ((bxx::multi_array<int32_t>*)(left()->value().array)); 
            break;
        case I64_A:
            delete ((bxx::multi_array<int64_t>*)(left()->value().array));
            break;
        case R32_A:
            delete ((bxx::multi_array<float>*)(left()->value().array));
            break;
        case R64_A:
            delete ((bxx::multi_array<double>*)(left()->value().array));
            break;
        case BOOL_A:
            delete ((bxx::multi_array<bool>*)(left()->value().array));
            break;
        }       
    }
}
string Query::dot_label(void) { return "Query"; }

Neg::Neg(Node* left) : Node(left) {}
string Neg::dot_label(void) { return "Neg"; }

Inv::Inv(Node* left) : Node(left) {}
string Inv::dot_label(void) { return "Inv"; }

Add::Add(Node* left, Node* right) : Node(left, right) {}

void Add::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_add(this, left(), right());
}
string Add::dot_label(void) { return "Add"; }

Sub::Sub(Node* left, Node* right) : Node(left, right) {}
void Sub::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_sub(this, left(), right());
}
string Sub::dot_label(void) { return "Sub"; }

Mul::Mul(Node* left, Node* right) : Node(left, right) {}
void Mul::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_mul(this, left(), right());
}
string Mul::dot_label(void) { return "Mul"; }

Mod::Mod(Node* left, Node* right) : Node(left, right) {}
void Mod::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_mod(this, left(), right());
}
string Mod::dot_label(void) { return "Mod"; }

Pow::Pow(Node* left, Node* right) : Node(left, right) {}
void Pow::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_pow(this, left(), right());
}
string Pow::dot_label(void) { return "Pow"; }

Div::Div(Node* left, Node* right) : Node(left, right) {}
void Div::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_div(this, left(), right());
}
string Div::dot_label(void) { return "Div"; }

LThan::LThan(Node* left, Node* right) : Node(left, right) {
    if ((vtype() & SCALAR)>0) {
        vtype(BOOL);
    } else {
        vtype(BOOL_A);
    }
}
void LThan::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_lthan(this, left(), right());
}
string LThan::dot_label(void) { return "LThan"; }

As::As(Node* left, Node* right) : Node(left, right) {

    switch(left->vtype()) {
    case BOOL:  vtype(BOOL_A); break;
    case I32: vtype(I32_A); break;
    case I64: vtype(I64_A); break;
    case R32: vtype(R32_A); break;
    case R64: vtype(R64_A); break;

    case STR:       throw logic_error("Array of strings is unsupported.");

    case BOOL_A:    throw logic_error("Array of arrays is unsupported.");
    case I32_A:   throw logic_error("Array of arrays is unsupported.");
    case I64_A:   throw logic_error("Array of arrays is unsupported.");
    case R32_A:  throw logic_error("Array of arrays is unsupported.");
    case R64_A:  throw logic_error("Array of arrays is unsupported.");
    case UNDEFINED: throw logic_error("Cannot construct array of undefined.");
    }

    uint64_t rank = 0;
    int64_t shape[16];
    Node* shapeNode = this;
    uint64_t nelements = 1;
    while (shapeNode->right()) {
        shapeNode = shapeNode->right();
        shape[rank] = shapeNode->left()->value().i64;
        nelements *= shape[rank];
        rank++;
    }

    //
    // NOTE: At this stage the multi_array<T> type does not matter since
    //       the type is not used until link() is called.
    //       Linkage is postponed until evaluation.
    _value.array = new bxx::multi_array<double>((const uint64_t)rank, (const int64_t*)shape);

}
void As::eval(void)
{
    left()->eval();

    switch(vtype()) {
    case BOOL_A:
        ((bxx::multi_array<bool>*)(_value.array))->link();
        *((bxx::multi_array<bool>*)(_value.array)) = left()->value().boolean;
        break;
    case I32_A:
        ((bxx::multi_array<int32_t>*)(_value.array))->link();
        *((bxx::multi_array<int32_t>*)(_value.array)) = left()->value().i32;
        break;
    case I64_A:
        ((bxx::multi_array<int64_t>*)(_value.array))->link();
        *((bxx::multi_array<int64_t>*)(_value.array)) = left()->value().i64;
        break;
    case R32_A:
        ((bxx::multi_array<float>*)(_value.array))->link();
        *((bxx::multi_array<float>*)(_value.array)) = left()->value().r32;
        break;
    case R64_A:
        ((bxx::multi_array<double>*)(_value.array))->link();
        *((bxx::multi_array<double>*)(_value.array)) = left()->value().r64;
        break;
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

Attr::Attr(Node* left, Node* right) : Node(left, right) {
    left->vtype(right->vtype());
    vtype(right->vtype());
}
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
        nls::Node* stmtList = new nls::StmtList(right);
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
