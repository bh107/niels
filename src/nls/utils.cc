#include <nls/utils.hh>

using namespace std;

namespace nls {

string value_type_error(Variant* res, Variant* left)
{
    stringstream ss;
    ss << "Unsupported types for operator: ";
    ss << value_type_text(res->value_type)      << ", ";
    ss << value_type_text(left->value_type)     << ".";
    return ss.str();
}

string value_type_error(Variant* res, Variant* left, Variant* right)
{
    stringstream ss;
    ss << "Unsupported types for operator: ";
    ss << value_type_text(res->value_type)      << ", ";
    ss << value_type_text(left->value_type)     << ", ";
    ss << value_type_text(right->value_type)    << ".";
    return ss.str();
}

string value_type_text(ValueType value_type)
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

string variant_text(Variant variant)
{
    stringstream ss;
    switch(variant.value_type) {
    case NLS_UND: ss << "undefined"; break;

    case NLS_BUL: ss << variant.value.bul; break;
    case NLS_I32: ss << variant.value.i32; break;
    case NLS_I64: ss << variant.value.i64; break;
    case NLS_R32: ss << variant.value.r32; break;
    case NLS_R64: ss << variant.value.r64; break;

    case NLS_C64: ss << *variant.value.c64; break;
    case NLS_C128: ss << *variant.value.c128; break;

    case NLS_BUL_A: ss << *variant.value.bul_a; break;
    case NLS_I32_A: ss << *variant.value.i32_a; break;
    case NLS_I64_A: ss << *variant.value.i64_a; break;
    case NLS_R32_A: ss << *variant.value.r32_a; break;
    case NLS_R64_A: ss << *variant.value.r64_a; break;
    case NLS_C64_A: ss << *variant.value.c64_a; break;
    case NLS_C128_A: ss << *variant.value.c128_a; break;

    case NLS_STR: ss << *variant.value.str; break;

    default:
        ss << "FORGOTTEN ValueType!";
    }

    return ss.str();
}

/*
 *  Pretty print the ast rooted at the given node.
 */
string dot(ast::Node* node) {

    if (NULL==node) {
        return "";
    }
    return  node->dot()+node->dot_relation()+"\n" + \
            dot(node->left())                     + \
            dot(node->right());
}

}

