#include <iostream>
#include <fstream>
#include <nls/nls.hh>

using namespace std;

int main(int argc, char** argv) {

    // TOOD: REPL

    nls::Driver driver;

    string filename = "-";

    if (argc>1) {
        filename = argv[1];
    }
    
    driver.parse(filename);         // Parsing
                                    // TODO: Syntax check
                                    // TODO: Type check

    //driver.walk(driver.ast());      // TODO: Evaluation
   
    /* 
    if (argc>2) {                   // Dump dot
        ofstream dotfile(argv[2]);
        if (dotfile.is_open()) {
            dotfile << driver.dot() << endl;
            dotfile.close();
        }
    }
    */
}

