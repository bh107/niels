#ifndef NIELS_DRIVER_H
#define NIELS_DRIVER_H
#include "ast.hh"
#include "symboltable.hh"

namespace nls {

class Driver {
public:
    
    Driver(void);
    virtual ~Driver(void);

    int parse(const std::string& filename);

private:
    SymbolTable* _symbolTable;
    Node* _ast;

    std::string _filename;
    bool trace_parsing;
    bool trace_scanning;

};

}
#endif

