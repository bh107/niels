#include <iostream>
#include <fstream>
#include <nls/nls.hh>

using namespace std;

int main(int argc, char** argv) {

    nls::Driver driver;             // TODO: REPL

    string filename = "-";          // Default to stdin
    if (argc>1) {
        filename = argv[1];
    }
    
    driver.parse(filename);         // Parsing

    if (argc>2) {                   // Dump dot
        ofstream dotfile(argv[2]);
        if (dotfile.is_open()) {
            dotfile << driver.dot() << endl;
            dotfile.close();
        }
    }
                                    // TODO: Syntax check
                                    // TODO: Type check

    driver.eval();                  // TODO: Evaluation

}

