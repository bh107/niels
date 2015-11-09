#ifndef NIELS_SYMBOLTABLE_H
#define NIELS_SYMBOLTABLE_H
#include <sstream>
#include <string>
#include <map>
#include <ast.hh>

namespace nls {

/**
 *  SymbolTable!
 */
class SymbolTable {
public:
    SymbolTable(void);

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

