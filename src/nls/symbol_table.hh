#ifndef NLS_SYMBOL_TABLE_HH
#define NLS_SYMBOL_TABLE_HH
#include <map>
#include <nls/ast/node.hh>
namespace nls {

class SymbolTable {
public:
    SymbolTable(void);

    ~SymbolTable(void);

    /*
     * Returns a pointer to the symbol if it exists
     * and NULL otherwise.
     */
    Variant get(const std::string& name);

    void put(std::string name, Variant val);

    std::string& scope(void);

    void scope(const std::string& name);

private:
    std::string _scope;
    std::map<std::string, Variant> _symbols;
};

}

#endif //NLS_SYMBOL_TABLE_HH
