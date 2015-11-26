#include <sstream>
#include <map>
#include <nls/symbol_table.hh>

using namespace std;

namespace nls {

SymbolTable::SymbolTable(void) : _symbols(), _scope("root") { }

SymbolTable::~SymbolTable(void) { }

Variant* SymbolTable::get(string name)
{
    stringstream scopedName;
    scopedName << scope() << "::" << name;

    map<string, Variant*>::iterator it = _symbols.find(scopedName.str());
    if (it != _symbols.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

void SymbolTable::put(string name, Variant* val)
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
