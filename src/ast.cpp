#include "ast.h"

using namespace std;

namespace nir {

//
// Pretty printers
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

std::string Operator_text(Operator op)
{
    switch(op) {
        case NOOP: return "NOOP";
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case POW: return "POW";
        case DIV: return "DIV";
        case NEG: return "NEG";
        case INV: return "INV";
        case QUERY: return "QUERY";
        case ASSIGN: return "ASSIGN";
        case ALIAS: return "ALIAS";
        default: return "<BROKEN:Operator_text>";
    }
}

std::string Structure_text(Structure structure)
{
    switch(structure) {
        case NOSTRUCT: return "NOSTRUCT";
        case SHAPE: return "SHAPE";
        case RANGE: return "RANGE";
        default: return "<BROKEN:Structure_text>";
    }
}

std::string Vclass_text(Vclass vclass)
{
    switch(vclass) {
        case NOVAL: return "NOVAL";
        case I32: return "I32";
        case I64: return "I64";
        case R32: return "R32";
        case R64: return "R64";
        case STRING: return "STRING";
        case BOOL: return "BOOL";
        case IDENT: return "IDENT";
        default: return "<BROKEN:Vclass_text>";
    }
}

std::string Vtype_text(Vclass vclass, Vtype vtype)
{
    stringstream ss;
    switch(vclass) {
        case NOVAL: ss << "NOVAL"; break;
        case I32: ss << vtype.i32; break;
        case I64: ss << vtype.i64; break;
        case R32: ss << vtype.r32; break;
        case R64: ss << vtype.r64; break;
        case STRING: ss << vtype.str; break;
        case BOOL: ss << vtype.boolean; break;
        case IDENT: ss << vtype.str; break;
        default: ss << "<BROKEN:Vtype_text>"; break;
    }
    return ss.str();
}

//
// The jizzle
//
Node::Node(void)
    : _operator(NOOP), _structure(NOSTRUCT), _vclass(NOVAL),  _left(NULL), _right(NULL) {}

Node::Node(Vclass vclass, int32_t val) 
    : _operator(NOOP), _structure(NOSTRUCT), _vclass(vclass),  _left(NULL), _right(NULL)
{
    _vtype.i32 = val;
}

Node::Node(Vclass vclass, int64_t val) 
    : _operator(NOOP), _structure(NOSTRUCT), _vclass(vclass),  _left(NULL), _right(NULL)
{
    _vtype.i64 = val;
}

Node::Node(Vclass vclass, float val) 
    : _operator(NOOP), _structure(NOSTRUCT), _vclass(vclass),  _left(NULL), _right(NULL)
{
    _vtype.r32 = val;
}

Node::Node(Vclass vclass, double val) 
    : _operator(NOOP), _structure(NOSTRUCT), _vclass(vclass),  _left(NULL), _right(NULL)
{
    _vtype.r64 = val;
}

Node::Node(Vclass vclass, bool val) 
    : _operator(NOOP), _structure(NOSTRUCT), _vclass(vclass),  _left(NULL), _right(NULL)
{
    _vtype.boolean = val;
}

Node::Node(Vclass vclass, char* val) 
    : _operator(NOOP), _structure(NOSTRUCT), _vclass(vclass),  _left(NULL), _right(NULL)
{
    _vtype.str = val;
}

Node::Node(Operator op, Node* right) 
    : _operator(op), _structure(NOSTRUCT), _vclass(NOVAL),  _left(NULL), _right(right) {}

Node::Node(Operator op, Node* left, Node* right) 
    : _operator(op), _structure(NOSTRUCT), _vclass(NOVAL),  _left(left), _right(right) {}

string Node::text(void)
{
    stringstream ss;
    ss << "NODE" << endl;
    return ss.str();
}

string Node::dot(void)
{
    stringstream ss;
    string shape = _vclass ? "diamond" : "box";
    string label = _vclass ? Vtype_text(_vclass, _vtype) : Operator_text(_operator);
    ss  << "N" << this << "["
        << "label=" << label << ","
        << "shape=" << shape
        << "]" << endl;

    if (_left) {
        ss << "N" << this << " -> N" << _left << endl;
    }
    if (_right) {
        ss << "N" << this << " -> N" << _right << endl;
    }
    return ss.str();
}

Node* Node::left(void)
{
    return _left;
}

Node* Node::right(void)
{
    return _right;
}

void eval(Node* node) { }


}

