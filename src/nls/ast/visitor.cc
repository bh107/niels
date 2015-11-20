#include <sstream>
#include <nls/ast/visitor.hh>

using namespace std;
namespace nls {
namespace ast {

Visitor::Visitor(Driver& driver) : _driver(driver)
{

}

Visitor::~Visitor(void)
{

}

void Visitor::visit(Node* node)
{
    cout << "General case" << endl;
}

}}

