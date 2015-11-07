#ifndef NIELS_UTILS_H
#define NIELS_UTILS_H
#include <sstream>
#include "ast.hh"
#include "symboltable.hh"

namespace nls {

template <typename T>
std::string tos(T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

void eval(Node* node);

std::string dot(Node* node);
std::string dot(SymbolTable* node);

}
#endif

