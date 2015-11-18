#include <typeinfo>
#include <exception>
#include <iomanip>
#include <ast.hh>
#include <driver.hh>

using namespace std;

namespace nls {

Comment::Comment(const char* comment) : Node() {
    _value.str = new string(comment);
    _vtype = NLS_STR;
}
string Comment::dot_label(void) { return "Comment"; }
string Comment::dot_shape(void) { return "box"; }

Str::Str(const char* val) : Node() {
    _value.str = new string(val);
    _vtype = NLS_STR;
}
string Str::dot_label(void) { return *_value.str; }
string Str::dot_shape(void) { return "house"; }
string Str::dot_color(void) { return "#5aae61"; }

Ident::Ident(const std::string& name) : Node()
{
    _name = name;
}
void Ident::eval(Driver& env)
{
    Node* identNode = env.symbolTable().getIdent(this);
    if (identNode) {
        _value = identNode->value();
    }
}
string Ident::dot_label(void) { return name(); }
string Ident::dot_shape(void) { return "hexagon"; }
string Ident::dot_color(void) { return "#fee0b6"; }

Query::Query(Node* left) : Node(left) {}
void Query::eval(Driver& env)
{
    Node* exprNode = left();
    
    cout << exprNode->txt() << endl;
    
    switch(exprNode->stype()) {
    case VAR:
    case FUN:
    case REC:
        break;
    case EXPR:
        switch(exprNode->vtype()) {
        case NLS_BUL_A:
            delete exprNode->value().bul_a;
            exprNode->value().bul_a = NULL;
            break;

        case NLS_I32_A:
            delete exprNode->value().i32_a;
            exprNode->value().r32_a = NULL;
            break;
        case NLS_I64_A:
            delete exprNode->value().i64_a;
            exprNode->value().r64_a = NULL;
            break;

        case NLS_R32_A:
            delete exprNode->value().r32_a;
            exprNode->value().r32_a = NULL;
            break;
        case NLS_R64_A:
            delete exprNode->value().r64_a;
            exprNode->value().r64_a = NULL;
            break;

        case NLS_C64_A:
            delete exprNode->value().c64_a;
            exprNode->value().c64_a = NULL;
            break;
        case NLS_C128_A:
            delete exprNode->value().c128_a;
            exprNode->value().c128_a = NULL;
            break;
        }

        break;
    default:
        break;
    }
}
string Query::dot_label(void) { return "Query"; }

As::As(Node* left, Node* right) : Node(left, right) {

    switch(left->vtype()) { // Derive the type
    case NLS_BUL: vtype(NLS_BUL_A); break;
    case NLS_I32: vtype(NLS_I32_A); break;
    case NLS_I64: vtype(NLS_I64_A); break;
    case NLS_R32: vtype(NLS_R32_A); break;
    case NLS_R64: vtype(NLS_R64_A); break;

    case NLS_STR: throw logic_error("Array of strings is unsupported.");

    case NLS_BUL_A: throw logic_error("Array of arrays is unsupported.");
    case NLS_I32_A: throw logic_error("Array of arrays is unsupported.");
    case NLS_I64_A: throw logic_error("Array of arrays is unsupported.");
    case NLS_R32_A: throw logic_error("Array of arrays is unsupported.");
    case NLS_R64_A: throw logic_error("Array of arrays is unsupported.");
    case NLS_UND: throw logic_error("Cannot construct array of undefined.");
    }
    stype(EXPR);
}
void As::eval(Driver& env)
{
    uint64_t rank = 0;  // Process meta-data
    int64_t shape[16];
    Node* shapeNode = this;
    uint64_t nelements = 1;
    while (shapeNode->right()) {
        shapeNode = shapeNode->right();
        shape[rank] = shapeNode->left()->value().i64;
        nelements *= shape[rank];
        rank++;
    }

    switch(vtype()) {   // Construct the array operation
    case NLS_BUL_A:
        _value.bul_a = new bul_a_type((const uint64_t)rank, (const int64_t*)shape);
        _value.bul_a->link();
        *(_value.bul_a) = left()->value().bul;
        break;
    case NLS_I32_A:
        _value.i32_a = new i32_a_type((const uint64_t)rank, (const int64_t*)shape);
        _value.i32_a->link();
        *(_value.i32_a) = left()->value().i32;
        break;
    case NLS_I64_A:
        _value.i64_a = new i64_a_type((const uint64_t)rank, (const int64_t*)shape);
        _value.i64_a->link();
        *(_value.i64_a) = left()->value().i64;
        break;
    case NLS_R32_A:
        _value.r32_a = new r32_a_type((const uint64_t)rank, (const int64_t*)shape);
        _value.r32_a->link();
        *(_value.r32_a) = left()->value().r32;
        break;
    case NLS_R64_A:
        _value.r64_a = new r64_a_type((const uint64_t)rank, (const int64_t*)shape);
        _value.r64_a->link();
        *(_value.r64_a) = left()->value().r64;
        break;
    }
}
string As::dot_label(void) { return "As"; }

Assign::Assign(Node* left, Node* right) : Node(left, right) {

    if (left->defined() && (left->vtype() != right->vtype())) {
        stringstream ss;
        ss << "Assigning " << right->name();
        ss << " to " << left->name();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (left->known() && (left->stype() != VAR)) {
        stringstream ss;
        ss << "Assigning " << right->name();
        ss << " to " << left->name();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (!left->defined()) {   // Derive the type of IDENT from right
        left->vtype(right->vtype());    
        left->stype(VAR);
    }
}
void Assign::eval(Driver& env)
{
    Node* var = env.symbolTable().getIdent(left());
    var->value(right()->value());
}
string Assign::dot_label(void) { return "Assign"; }

Alias::Alias(Node* left, Node* right) : Node(left, right) {
    if ((left->vtype() != NLS_UND) && (left->vtype() != right->vtype())) {
        stringstream ss;
        ss << "Aliasing " << right->txt();
        ss << " to " << left->name() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if ((left->stype() != UNKNOWN) && (left->stype() != VAR)) {
        stringstream ss;
        ss << "Aliasing " << right->txt();
        ss << " to " << left->name() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (left->vtype() == NLS_UND) {   // Derive the type of IDENT from right
        left->vtype(right->vtype());    
        left->stype(VAR);
    }
    vtype(left->vtype());               // IDENT determines type
}
string Alias::dot_label(void) { return "Alias"; }

Range::Range(bool lb_open, bool ub_open, Node* left, Node* right)
    : Node(left, right), _lb_open(lb_open), _ub_open(ub_open)
{
    vtype(NLS_I64_A);
    stype(EXPR);

    if ((left->vtype() != NLS_I64) || (right->vtype() != NLS_I64)) {
        throw logic_error("Booo");
    }
}
void Range::eval(Driver& env)

{
    int64_t lower = left()->value().i64;
    if (_lb_open) {
        lower++;
    }
    int64_t upper = right()->value().i64;
    if (_ub_open) {
        upper--;
    }

    int64_t nelements = upper-lower+1;
    uint64_t rank = 1;
    int64_t shape[16];
    shape[0] = nelements;

    value().i64_a = new bxx::multi_array<int64_t>((const uint64_t)rank, (const int64_t*)shape);
    value().i64_a->link();
    bxx::bh_range(*value().i64_a);
    value().i64_a->setTemp(false);

    bxx::bh_add(*value().i64_a, *value().i64_a, lower);
}
string Range::dot_label(void) { return "Range"; }
string Range::dot_shape(void) { return "trapezium"; }

Shape::Shape(Node* left) : Node(left) {}
string Shape::dot_label(void) { return "Shape"; }
string Shape::dot_shape(void) { return "trapezium"; }

Args::Args(Node* left) : Node(left) {}
string Args::dot_label(void) { return "Args"; }
string Args::dot_shape(void) { return "diamond"; }

Param::Param(Node* left, Node* right) : Node(left, right) {
    if (left->vtype() == NLS_UND) {   // Derive the type
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

Function::Function(Node* left) : Node(left) {
    stype(FUN);
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

Record::Record(Node* left) : Node(left) {
    stype(REC);
}
string Record::dot_label(void) { return "Record"; }
string Record::dot_shape(void) { return "parallelogram"; }

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
