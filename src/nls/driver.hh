#ifndef NLS_DRIVER_HH
#define NLS_DRIVER_HH
#include <string>
#include <nls/ast/node.hh>
#include <nls/symbol_table.hh>

namespace nls {

class Driver {
public:
    
    Driver(void);
    Driver(bool trace_scanning, bool trace_parsing, bool fewer_noops);
    virtual ~Driver(void);

    void scanBegin(void);
    void scanEnd(void);

    int parse(const std::string& filename);

    void error(const std::string& m);
    void error(int yylineno, const std::string& m);

    SymbolTable& symbolTable(void);

    void ast(ast::Node* node);
    ast::Node* ast(void);

    void createScope(const std::string& name);
    void exitScope(void);

    bool traceScanning(void);
    bool traceParsing(void);
    bool fewerNoops(void);

    std::string dot(void);

    void eval(void);

private:
    SymbolTable _symbolTable;
    ast::Node* _ast;

    std::string _filename;
    bool _trace_scanning;
    bool _trace_parsing;
    bool _fewer_noops;
};

}
#endif

