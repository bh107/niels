#include <map>
#include <nls/symboltable.hh>
#include <nls/utils.hh>

using namespace std;

namespace nls {

SymbolTable::SymbolTable(void) : _symbols(), _scope("root") {}

SymbolTable::~SymbolTable(void)
{
    // Delete nodes in symboltable
    for(map<string, ast::Node*>::iterator it=_symbols.begin();
        it != _symbols.end();
        ++it) {
        delete it->second;
    }
}

ast::Node* SymbolTable::getIdent(ast::Node* node)
{
    stringstream scopedIdent;
    scopedIdent << scope() << "::" << node->name();

    map<string, ast::Node*>::iterator it = _symbols.find(scopedIdent.str());
    if (it != _symbols.end()) {
        node->stype(it->second->stype());
        node->vtype(it->second->vtype());
        return it->second;
    } else {
        return NULL;
    }
}

ast::Node* SymbolTable::lookup(string ident)
{
    stringstream scopedIdent;
    scopedIdent << scope() << "::" << ident;

    map<string, ast::Node*>::iterator it = _symbols.find(scopedIdent.str());
    if (it != _symbols.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

void SymbolTable::put(string ident, ast::Node* node)
{
    stringstream scopedIdent;
    scopedIdent << scope() << "::" << ident;

    _symbols[scopedIdent.str()] = node;
}

void SymbolTable::scope(const std::string& val)
{
    _scope = val;
}

std::string& SymbolTable::scope(void)
{
    return _scope;
}

string SymbolTable::dot(void)
{
    stringstream ss;
    ss << "subgraph cluster" << this <<  "{" << endl;
    ss << "SymbolTableStart" << this << "[label=\"<f0>|<f1>SymbolTable|<f2>\",shape=record];" << endl;
    for (map<string, ast::Node*>::iterator it=symbols().begin(); it!=symbols().end(); ++it) {
        ss << "\"" << it->second << "\" [" << endl;
        ss << "label=" << "\"<f0>" << it->first << " | " 
           << VType_text(it->second->vtype()) << " | " 
           << SType_text(it->second->stype()) << " \"" << endl;
        ss << "shape=\"record\"" << endl;
        ss << "]; " <<  endl;
    }
    ss << "SymbolTableEnd" << this << "[label=\"<f0>|<f1>SymbolTable|<f2>\",shape=record];" << endl;

    ss << "SymbolTableStart" << this << ":f0 -- " << endl;
    for (map<string, ast::Node*>::iterator it=symbols().begin(); it!=symbols().end(); ++it) {
        ss << "\"" << it->second << "\":f0 -- ";
    }
    ss << "SymbolTableEnd" << this << ":f0" << endl;
    ss << "}" << endl;

    return ss.str();
}

map<std::string, ast::Node*>& SymbolTable::symbols(void)
{
    return _symbols;
}

}

