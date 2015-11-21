#include <sstream>
#include <nls/ast/evaluator.hh>

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

}}

