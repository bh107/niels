#include <iostream>
#include <fstream>
#include <driver.hh>
#include <utils.hh>

using namespace std;

int main(int argc, char** argv) {

    nls::Driver driver;

    string filename = "-";

    if (argc>1) {
        filename = argv[1];
    }
   
    driver.parse(filename);    // Parsing
    // TODO: Syntax check
    // TODO: Type check

    driver.eval(driver.ast());    // TODO: evaluation
    
    if (argc>2) {           // Dump dot
        ofstream dotfile(argv[2]);
        if (dotfile.is_open()) {
            dotfile << driver.dot() << endl;
            dotfile.close();
        }
    }
}

