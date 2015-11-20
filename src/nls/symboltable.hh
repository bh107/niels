#ifndef NLS_SYMBOLTABLE_HH
#define NLS_SYMBOLTABLE_HH
#include <string>
#include <map>
#include <nls/ast/node.hh>

namespace nls {

/**
 *  SymbolTable!
 */
class SymbolTable {
public:
    SymbolTable(void);
    ~SymbolTable(void);

    Node* lookup(std::string ident);

    /**
     * Assign stype and vtype to the given Ident node
     * of the defining Ident instance in symbol table.
     *
     * @return NULL When no defining ident exist.
     * @return Pointer to defining Ident instance.
     */
    Node* getIdent(Node* node);
    void put(std::string, Node* node);
    std::map<std::string, Node*>& symbols(void);

    std::string dot(void);

    std::string& scope(void);
    void scope(const std::string& val);

private:
    std::string _scope;

    std::map<std::string, Node*> _symbols;    
};

}
#endif

