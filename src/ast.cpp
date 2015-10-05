#include "ast.h"

using namespace std;

Node::Node(void) : _left(NULL), _right(NULL) {}

string Node::text(void)
{
    stringstream ss;
    ss << "NODE" << endl;
    return ss.str();
}

void Node::left(Node* left)
{
    _left = left;
}

void Node::right(Node* right)
{
    _right = right;
}

Node* Node::left(void)
{
    return _left;
}

Node* Node::right(void)
{
    return _right;
}

void eval(Node* node)
{
    if (node) {
        cout << node->text();
        if (node->left()) {
            eval(node->left());
        }
        if (node->right()) {
            eval(node->right());
        }
    }    
}
