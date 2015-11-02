#include <map>
#include "symboltable.h"

using namespace std;

namespace nir{

SymbolTable::SymbolTable(void) : _symbols() {}

SymbolTable* SymbolTable::parent(void)
{
    return _parent;
}

void SymbolTable::parent(SymbolTable* symbolTable)
{
    _parent = symbolTable;
}

SymbolTable* SymbolTable::child(void)
{
    return _child;
}

void SymbolTable::child(SymbolTable* symbolTable)
{
    _child = symbolTable;
}

SymbolTable* SymbolTable::sibling(void)
{
    return _sibling;
}

void SymbolTable::sibling(SymbolTable* symbolTable)
{
    _sibling = symbolTable;
}

Node* SymbolTable::getIdent(Node* ident)
{
    map<string, Node*>::iterator it = _symbols.find(ident->str());
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


string SymbolTable::dot(void)
{
    stringstream ss;
    ss << "subgraph cluster" << this <<  "{" << endl;
    ss << "SymbolTableStart[label=\"<f0>|<f1>SymbolTable|<f2>\",shape=record];" << endl;
    for (map<string, Node*>::iterator it=symbols().begin(); it!=symbols().end(); ++it) {
        ss << "\"" << it->second << "\" [" << endl;
        ss << "label=" << "\"<f0>" << it->first << " | " 
           << VType_text(it->second->vtype()) << " | " 
           << SType_text(it->second->stype()) << " \"" << endl;
        ss << "shape=\"record\"" << endl;
        ss << "]; " <<  endl;
    }
    ss << "SymbolTableEnd[label=\"<f0>|<f1>SymbolTable|<f2>\",shape=record];" << endl;

    ss << "SymbolTableStart:f0 -- " << endl;
    for (map<string, Node*>::iterator it=symbols().begin(); it!=symbols().end(); ++it) {
        ss << "\"" << it->second << "\":f0 -- ";
    }
    ss << "SymbolTableEnd:f0" << endl;
    ss << "}" << endl;

    return ss.str();
}

map<std::string, Node*>& SymbolTable::symbols(void)
{
    return _symbols;
}

}

