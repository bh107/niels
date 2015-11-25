#ifndef NIELS_DRIVER_H
#define NIELS_DRIVER_H
#include <ast_node.hh>
#include <symboltable.hh>

namespace nls {

class Driver {
public:
    
    Driver(void);
    Driver(bool trace_scanning, bool trace_parsing, bool fewer_noops);
    virtual ~Driver(void);

    void scanBegin(void);
    void scanEnd(void);

    int parse(const std::string& filename);

    void walk(Node* node);

    void error(const std::string& m);
    void error(int yylineno, const std::string& m);

    SymbolTable& symbolTable(void);

    void ast(Node* node);
    Node* ast(void);

    void createScope(const std::string& name);
    void exitScope(void);

    bool traceScanning(void);
    bool traceParsing(void);
    bool fewerNoops(void);

    std::string dot(void);

    void eval(void);

private:
    SymbolTable _symbolTable;
    Node* _ast;

    std::string _filename;
    bool _trace_scanning;
    bool _trace_parsing;
    bool _fewer_noops;
};

}
#endif

