#include <typeinfo>
#include <exception>
#include <iomanip>
#include <nls/ast/stmt.hh>
#include <nls/driver.hh>

using namespace std;

namespace nls {
namespace ast {

Collection::Collection(Node* left, Node* right) : Node(left, right) { }
string Collection::dot_label(void) { return "Collection"; }
string Collection::dot_shape(void) { return "box"; }

Module::Module(Node* left, Node* right) : Node(left, right) { }
string Module::dot_label(void) { return "Module"; }
string Module::dot_shape(void) { return "box"; }

Query::Query(Node* left) : Node(left) {}
void Query::eval(Driver& env)
{
    Node* exprNode = left();

    //
    // Grab the textual representation of expr
    cout << exprNode->txt() << endl;

    //
    // Cleanup    
    switch(exprNode->stype()) {
    case EXPR:
        switch(exprNode->vtype()) {
        case NLS_BUL_A:
            delete exprNode->value().bul_a;
            exprNode->value().bul_a = NULL;
            break;

        case NLS_I32_A:
            delete exprNode->value().i32_a;
            exprNode->value().r32_a = NULL;
            break;
        case NLS_I64_A:
            delete exprNode->value().i64_a;
            exprNode->value().r64_a = NULL;
            break;

        case NLS_R32_A:
            delete exprNode->value().r32_a;
            exprNode->value().r32_a = NULL;
            break;
        case NLS_R64_A:
            delete exprNode->value().r64_a;
            exprNode->value().r64_a = NULL;
            break;

        case NLS_C64_A:
            delete exprNode->value().c64_a;
            exprNode->value().c64_a = NULL;
            break;
        case NLS_C128_A:
            delete exprNode->value().c128_a;
            exprNode->value().c128_a = NULL;
            break;
        }

        break;
    default:
        break;
    }
}
string Query::dot_label(void) { return "Query"; }

Assign::Assign(Node* left, Node* right) : Node(left, right) {

    if (left->defined() && (left->vtype() != right->vtype())) {
        stringstream ss;
        ss << "Assigning " << right->name();
        ss << " to " << left->name();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (left->known() && (left->stype() != VAR)) {
        stringstream ss;
        ss << "Assigning " << right->name();
        ss << " to " << left->name();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (!left->defined()) {   // Derive the type of IDENT from right
        left->vtype(right->vtype());    
        left->stype(VAR);
    }
}
void Assign::eval(Driver& env)
{
    Node* var = env.symbolTable().getIdent(left());
    var->value(right()->value());
}
string Assign::dot_label(void) { return "Assign"; }

Alias::Alias(Node* left, Node* right) : Node(left, right) {
    if ((left->vtype() != NLS_UND) && (left->vtype() != right->vtype())) {
        stringstream ss;
        ss << "Aliasing " << right->txt();
        ss << " to " << left->name() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if ((left->stype() != UNKNOWN) && (left->stype() != VAR)) {
        stringstream ss;
        ss << "Aliasing " << right->txt();
        ss << " to " << left->name() << " " << left->txt();
        ss << " is not supported.";

        throw logic_error(ss.str());
    }

    if (left->vtype() == NLS_UND) {   // Derive the type of IDENT from right
        left->vtype(right->vtype());    
        left->stype(VAR);
    }
    vtype(left->vtype());               // IDENT determines type
}
string Alias::dot_label(void) { return "Alias"; }

Args::Args(Node* left) : Node(left) {
    stype(ARG);
}
string Args::dot_label(void) { return "Args"; }
string Args::dot_shape(void) { return "diamond"; }

Param::Param(Node* left, Node* right) : Node(left, right) {
    if (left->vtype() == NLS_UND) {   // Derive the type
        left->vtype(right->vtype());    
    }
    vtype(right->vtype());
    stype(PARAM);
}
string Param::dot_label(void) { return "Param"; }
string Param::dot_shape(void) { return "diamond"; }

Params::Params(Node* left) : Node(left) {}
string Params::dot_label(void) { return "Params"; }
string Params::dot_shape(void) { return "diamond"; }

FunctionDef::FunctionDef(Node* left) : Node(left) {
    stype(FUN_DEF);
}
string FunctionDef::dot_label(void) { return "FunctionDef"; }
string FunctionDef::dot_shape(void) { return "parallelogram"; }

FunctionBody::FunctionBody(Node* left, Node* right) : Node(left, right) {}
string FunctionBody::dot_label(void) { return "FunctionBody"; }
string FunctionBody::dot_shape(void) { return "parallelogram"; }

Return::Return(Node* left) : Node(left) {}
string Return::dot_label(void) { return "Return"; }
string Return::dot_shape(void) { return "box"; }

Attr::Attr(Node* left, Node* right) : Node(left, right) {
    left->vtype(right->vtype());
    vtype(right->vtype());
    stype(ATTR);
}
string Attr::dot_label(void) { return "Attr"; }
string Attr::dot_shape(void) { return "parallelogram"; }

AttrList::AttrList(Node* left) : Node(left) {}
string AttrList::dot_label(void) { return "AttrList"; }
string AttrList::dot_shape(void) { return "parallelogram"; }

Record::Record(Node* left) : Node(left) {
    stype(REC);
    name(left->name());
}
string Record::dot_label(void) { return "Record"; }
string Record::dot_shape(void) { return "parallelogram"; }
string Record::txt(void) {
    stringstream ss;
    ss << "Record";
    return ss.str();
}

RecordDef::RecordDef(Node* left) : Node(left) {
    stype(REC_DEF);
    name(left->name());
}
string RecordDef::dot_label(void) { return "RecordDef"; }
string RecordDef::dot_shape(void) { return "parallelogram"; }
string RecordDef::txt(void) {
    stringstream ss;
    ss << "RecordDef";
    return ss.str();
}

Block::Block(Node* left, Node* right) : Node(left) {
    if (typeid(*right) != typeid(StmtList)) {
        nls::Node* stmtList = new nls::StmtList(right);
        stmtList->left(NULL);
        stmtList->right(right);
        this->right(stmtList);
    } else {
        this->right(right);
    }
}
string Block::dot_label(void) { return "Block"; }
string Block::dot_shape(void) { return "box"; }

While::While(Node* left, Node* right) : Node(left, right) { }
string While::dot_label(void) { return "While"; }
string While::dot_shape(void) { return "box"; }

When::When(Node* left, Node* right) : Node(left, right) { }
string When::dot_label(void) { return "When"; }
string When::dot_shape(void) { return "box"; }

WhenBool::WhenBool(Node* left, Node* right) : Node(left, right) { }
string WhenBool::dot_label(void) { return "WhenBool"; }
string WhenBool::dot_shape(void) { return "box"; }

Cases::Cases(Node* left) : Node(left) {}
Cases::Cases(Node* left, Node* right) : Node(left, right) {}
string Cases::dot_label(void) { return "Cases"; }
string Cases::dot_shape(void) { return "box"; }

Is::Is(Node* left, Node* right) : Node(left, right) { }
string Is::dot_label(void) { return "Is"; }
string Is::dot_shape(void) { return "box"; }

Otherwise::Otherwise(Node* left, Node* right) : Node(left, right) { }
string Otherwise::dot_label(void) { return "Otherwise"; }
string Otherwise::dot_shape(void) { return "box"; }

Undefined::Undefined(void) : Node() {}
string Undefined::dot_label(void) { return "Undefined"; }
string Undefined::dot_shape(void) { return "box"; }

Empty::Empty(void) : Node() {}
string Empty::dot_label(void) { return "Empty"; }
string Empty::dot_shape(void) { return "box"; }

Noop::Noop(void) : Node() {}
string Noop::dot_label(void) { return "Noop"; }
string Noop::dot_shape(void) { return "box"; }

StmtList::StmtList(Node* left) : Node(left) {}
StmtList::StmtList(Node* left, Node* right) : Node(left, right) {}
string StmtList::dot_label(void) { return "StmtList"; }
string StmtList::dot_shape(void) { return "box"; }

Import::Import(Node* left) : Node(left) {}
string Import::dot_label(void) { return "Import"; }
string Import::dot_shape(void) { return "box"; }

Program::Program(void) : Node() {}
string Program::dot_label(void) { return "Program"; }
string Program::dot_shape(void) { return "box"; }

Anon::Anon(void) : Node() {}
string Anon::dot_label(void) { return "Anon"; }
string Anon::dot_shape(void) { return "box"; }

}}

