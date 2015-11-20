#include <nls/ast/expr.hh>

using namespace std;

namespace nls {
namespace ast {

Comment::Comment(const char* comment) : Node() {
    _value.str = new string(comment);
    _vtype = NLS_STR;
}
string Comment::dot_label(void) { return "Comment"; }
string Comment::dot_shape(void) { return "box"; }

Str::Str(const char* val) : Node() {
    // Leading and ending " and ' are stripped from input
    _value.str = new string(val+1, strlen(val)-2);
    _vtype = NLS_STR;
    _stype = EXPR;
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

Call::Call(Node* left, Node* right) : Node(left, right) {}
string Call::dot_label(void) { return "Call"; }
string Call::dot_shape(void) { return "parallelogram"; }

Accessor::Accessor(Node* left, Node* right) : Node(left, right) { }
string Accessor::dot_label(void) { return "Accessor"; }
string Accessor::dot_shape(void) { return "parallelogram"; }


