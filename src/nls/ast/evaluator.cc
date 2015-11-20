#include <sstream>
#include <ast_evaluator.hh>

using namespace std;
namespace nls {
namespace ast {

Evaluator::Evaluator(Driver& driver) : Visitor(driver)
{

}

Evaluator::~Evaluator(void)
{

}

void Evaluator::visit(Node* node)
{
    cout << "General EVAL case" << endl;
}

void Evaluator::visit(Add* node)
{
    cout << "e + e" << endl;
}


}}

