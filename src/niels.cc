#include <iostream>
#include <fstream>
#include <driver.hh>
#include <ast.hh>
#include <utils.hh>
#include <ast_evaluator.hh>

using namespace std;

int main(int argc, char** argv) {

    nls::Driver driver;

    string filename = "-";

    if (argc>1) {
        filename = argv[1];
    }

    nls::Evaluator evaluator(driver);
    
    driver.parse(filename);    // Parsing
    // TODO: Syntax check
    // TODO: Type check

    driver.ast()->visit(evaluator);

    driver.walk(driver.ast());    // TODO: evaluation
    
    if (argc>2) {           // Dump dot
        ofstream dotfile(argv[2]);
        if (dotfile.is_open()) {
            dotfile << driver.dot() << endl;
            dotfile.close();
        }
    }
}

