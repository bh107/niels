#ifndef NIELS_DRIVER_H
#define NIELS_DRIVER_H
#include <ast.hh>
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

    void eval(Node* node);

    void error(const std::string& m);
    void error(int yylineno, const std::string& m);

    SymbolTable& symbolTable(void);

    void ast(Node* node);
    Node* ast(void);

    void scopeBegin();
    void scopeEnd();

    bool traceScanning(void);
    bool traceParsing(void);
    bool fewerNoops(void);

    std::string dot(void);

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

