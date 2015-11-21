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

    ast::Node* lookup(std::string ident);

    ast::Node* getIdent(ast::Node* node);
    void put(std::string, ast::Node* node);
    std::map<std::string, ast::Node*>& symbols(void);

    std::string dot(void);

    std::string& scope(void);
    void scope(const std::string& val);

private:
    std::string _scope;

    std::map<std::string, ast::Node*> _symbols;    
};

}
#endif

