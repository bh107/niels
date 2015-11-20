#include <sstream>
#include <ast_evaluator.hh>

using namespace std;
namespace nls {

Evaluator::Evaluator(Driver& driver) : AstVisitor(driver)
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


}

