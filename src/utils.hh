#ifndef NIELS_UTILS_H
#define NIELS_UTILS_H
#include <sstream>
#include <ast.hh>
#include <symboltable.hh>

namespace nls {

std::string SType_text(SType stype);
std::string VType_text(VType vtype);

template <typename T>
std::string tos(T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

std::string dot(Node* node);

}
#endif

