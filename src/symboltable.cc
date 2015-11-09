#include <map>
#include <symboltable.hh>
#include <utils.hh>

using namespace std;

namespace nls {

SymbolTable::SymbolTable(void) : _symbols(), _scope("root") {}

Node* SymbolTable::getIdent(Node* ident)
{
    map<string, Node*>::iterator it = _symbols.find(ident->name());
    if (it != _symbols.end()) {
        ident->stype(it->second->stype());
        ident->vtype(it->second->vtype());
        return it->second;
    } else {
        return NULL;
    }
}

Node* SymbolTable::lookup(string ident)
{
    map<string, Node*>::iterator it = _symbols.find(ident);
    if (it != _symbols.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

void SymbolTable::put(string ident, Node* node)
{
    _symbols[ident] = node;
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
    for (map<string, Node*>::iterator it=symbols().begin(); it!=symbols().end(); ++it) {
        ss << "\"" << it->second << "\" [" << endl;
        ss << "label=" << "\"<f0>" << it->first << " | " 
           << VType_text(it->second->vtype()) << " | " 
           << SType_text(it->second->stype()) << " \"" << endl;
        ss << "shape=\"record\"" << endl;
        ss << "]; " <<  endl;
    }
    ss << "SymbolTableEnd" << this << "[label=\"<f0>|<f1>SymbolTable|<f2>\",shape=record];" << endl;

    ss << "SymbolTableStart" << this << ":f0 -- " << endl;
    for (map<string, Node*>::iterator it=symbols().begin(); it!=symbols().end(); ++it) {
        ss << "\"" << it->second << "\":f0 -- ";
    }
    ss << "SymbolTableEnd" << this << ":f0" << endl;
    ss << "}" << endl;

    return ss.str();
}

map<std::string, Node*>& SymbolTable::symbols(void)
{
    return _symbols;
}

}

