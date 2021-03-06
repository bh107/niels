#ifndef NLS_AST_VISITOR_EVALUATOR_HH
#define NLS_AST_VISITOR_EVALUATOR_HH
#include <stack>
#include <nls/ast/visitor/visitor.hh>
#include <nls/symbol_table.hh>

namespace nls {
namespace ast {

class Evaluator : public Visitor {
public:
    Evaluator(void);
    Evaluator(SymbolTable& symbolTable);
    ~Evaluator(void);

    Variant peek(void);
    Variant pop(void);
    void push(Variant var);

    // Generated visit-declarations e.g. "void visit(Add& node);"
    #include <nls/ast/visitor/evaluator_visit_auto_hh.inc>

    void garbage(Variant& val);

private:
    std::stack<Variant> _stack;
    SymbolTable _symbolTable;

};

}}

#endif //NLS_AST_VISITOR_EVALUATOR_HH
