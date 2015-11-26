#include <nls/utils.hh>

using namespace std;

namespace nls {

string ValueType_text(ValueType value_type)
{
    switch(value_type) {
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

        default: return "FORGOTTEN ValueType!";
    }
}

/*
 *  Pretty print the ast rooted at the given node.
 */
/*
string dot(ast::Node* node) {

    if (NULL==node) {
        return "";
    }
    return  node->dot()+node->dot_relation()+"\n" + \
            dot(node->left())                     + \
            dot(node->right());
}
*/

}

