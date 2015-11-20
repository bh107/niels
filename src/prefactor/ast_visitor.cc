#include <sstream>
#include <ast_visitor.hh>

using namespace std;
namespace nls {

AstVisitor::AstVisitor(Driver& driver) : _driver(driver)
{

}

AstVisitor::~AstVisitor(void)
{

}

void AstVisitor::visit(Node* node)
{
    cout << "General case" << endl;
}

}

