#include "utils.hh"

using namespace std;

namespace nir {

void eval(Node* node) { }

//
// Pretty Printing
//
string dot(Node* node) {

    if (NULL==node) {
        return "";
    }
    return  node->dot()+node->dot_relation()+"\n" + \
            dot(node->left())                     + \
            dot(node->right());
}

string dot(SymbolTable* symbolTable) {

    if (NULL==symbolTable) {
        return "";
    }
    return symbolTable->dot()          + \
           dot(symbolTable->sibling()) + \
           dot(symbolTable->child());
}

}

