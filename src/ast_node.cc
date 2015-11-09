#include <string>
#include <ast_node.hh>
#include <utils.hh>

using namespace std;

namespace nls {

Node::Node(void)
    : _stype(UNKNOWN), _vtype(NLS_UND), _left(NULL), _right(NULL)
{
}
Node::Node(Node* left)
    : _stype(UNKNOWN), _vtype(left->vtype()), _left(left), _right(NULL)
{
}
Node::Node(Node* left, Node* right) : _stype(UNKNOWN), _vtype(NLS_UND), _left(left), _right(right)
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
    return _vtype != NLS_UND;
}

bool Node::known(void)
{
    return _stype != UNKNOWN;
}

Node::~Node(void) {
    if (vtype() == NLS_STR) {
        delete _value.str;
    }
}
string Node::dot_shape(void) { return "box"; }
string Node::dot_label(void) { return "b0rk3d"; }
string Node::dot_color(void) { return "#e0e0e0"; }

}

