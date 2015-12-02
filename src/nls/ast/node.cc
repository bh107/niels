#include <nls/ast/node.hh>
#include <nls/ast/nodes.hh>

using namespace std;
namespace nls {
namespace ast {

Node::Node(void) : _left(NULL), _right(NULL), _variant(NULL), _name("anonymous") { }

Node::Node(Node *left) : _left(left), _right(NULL), _variant(NULL), _name("anonymous") { }

Node::Node(Node *left, Node *right) : _left(left), _right(right), _variant(NULL), _name("anonymous") { }

Node::~Node(void)
{
    if (_variant) {
        delete _variant;
    }
}

Node *Node::left(void)
{
    return _left;
}

Node *Node::right(void)
{
    return _right;
}

void Node::left(Node* node)
{
    _left = node;
}

void Node::right(Node* node)
{
    _right = node;
}

void Node::append(Node* node)
{
    Node* next = this;
    while(next->right()) {
        next = next->right();
    }
    next->right(node);
}

void Node::variant(Variant *var)
{
    _variant = var;
}

Variant *Node::variant(void)
{
    return _variant;
}

void Node::name(std::string& val)
{
    _name = val;
}
std::string& Node::name(void)
{
    return _name;
}

string Node::dot_relation(void)
{
    stringstream ss;
    if (_left) {
        ss << "N" << this << " -- N" << _left
           << "[label=\"\""
           << "]" << endl;
    }
    if (_right) {
        ss << "N" << this << " -- N" << _right
           << "[label=\"\""
           << ",penwidth=3"
           << "]" << endl;

        if ((typeid(*this) == typeid(StmtList)) or \
            (typeid(*this) == typeid(CaseList))) {
            ss << "{rank=same; " << "N" << this << " N" << _right << " }" << endl;
        }
    }
    return ss.str();
}

string Node::dot(void)
{
    stringstream ss;
    ss  << "N" << this
        << "["
        << "label=\"" << dot_label() << "\","
        << "color=\"" << dot_color() << "\","
        << "shape=\"" << dot_shape() << "\","
        << "fontname=\"Courier\","
        << "style=\"filled\""
        << "]";

    return ss.str();
}
string Node::dot_shape(void) { return "box"; }
string Node::dot_label(void) { return "b0rk3d"; }
string Node::dot_color(void) { return "#e0e0e0"; }

}}
