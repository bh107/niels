#include <sstream>
#include <map>
#include <nls/symbol_table.hh>

using namespace std;

namespace nls {

SymbolTable::SymbolTable(void) : _scope("root"), _symbols() { }

SymbolTable::~SymbolTable(void) {
    
    for(map<string, Variant>::iterator it = _symbols.begin(); it!=_symbols.end(); ++it) {
        switch(it->second.value_type) {
        case NLS_BUL_A: delete it->second.value.bul_a; break;
        case NLS_I32_A: delete it->second.value.i32_a; break;
        case NLS_I64_A: delete it->second.value.i64_a; break;
        case NLS_R32_A: delete it->second.value.r32_a; break;
        case NLS_R64_A: delete it->second.value.r64_a; break;
        case NLS_C64_A: delete it->second.value.c64_a; break;
        case NLS_C128_A: delete it->second.value.c128_a; break;
        case NLS_STR: delete it->second.value.str; break;
        }
    }
}

Variant SymbolTable::get(const string& name)
{
    stringstream scopedName;
    scopedName << scope() << "::" << name;

    map<string, Variant>::iterator it = _symbols.find(scopedName.str());
    if (it != _symbols.end()) {
        return it->second;
    } else {
        return {0, NLS_UND};
    }
}

void SymbolTable::put(string name, Variant val)
{
    stringstream scopedName;
    scopedName << scope() << "::" << name;

    _symbols[scopedName.str()] = val;
}

void SymbolTable::scope(const std::string& val)
{
    _scope = val;
}

std::string& SymbolTable::scope(void)
{
    return _scope;
}

}
