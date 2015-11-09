#include "utils.hh"

using namespace std;

namespace nls {

string VType_text(VType vtype)
{
    switch(vtype) {
        case NLS_UND: return "undefined";
        
        case NLS_BUL: return "bool";
        case NLS_I32: return "i32";
        case NLS_I64: return "i64";
        case NLS_R32: return "r32";
        case NLS_R64: return "r64";
        case NLS_C64: return "c64";
        case NLS_C128: return "c128";

        case NLS_BUL_A: return "bool[]";
        case NLS_I32_A: return "i32[]";
        case NLS_I64_A: return "i64[]";
        case NLS_R32_A: return "r32[]";
        case NLS_R64_A: return "r64[]";
        case NLS_C64_A: return "c64[]";
        case NLS_C128_A: return "c128[]";

        case NLS_STR: return "string";
    }
}

string SType_text(SType stype)
{
    switch(stype) {
        case VAR:       return "variable";
        case FUNC:      return "function";
        case MOD:       return "module";
        case COLL:      return "collection";
        case UNKNOWN:   return "unknown";
    }
}


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

}

