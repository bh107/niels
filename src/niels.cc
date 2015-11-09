#include <iostream>
#include <driver.hh>
#include <utils.hh>

using namespace std;

int main(int argc, char** argv) {

    nls::Driver drv;

    drv.parse("input.nls");
   
    if (argc>2) {
        cout << "graph {" << endl;
        cout << "graph[ordering=out]" << endl;
        
        nls::SymbolTable* table = drv.symbolTable();
        while(table->parent()) {  // Go to top-level
            table = table->parent();
        }
        cout << nls::dot(table) << endl;
        cout << nls::dot(drv.ast()) << endl;
        cout << "}" << endl;
    }
}

