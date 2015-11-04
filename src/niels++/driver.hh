#ifndef NIELS_DRIVER_HH
#define NIELS_DRIVER_HH

#include <string>
#include <map>
#include <parser.hh>
#include <location.hh>

#define YY_DECL \
  nls::Parser::symbol_type yylex (nls::Driver& driver)
YY_DECL;

namespace nls {

class Driver {
public:
    Driver();

    virtual ~Driver();

    // Handling the scanner.
    void scan_begin();

    void scan_end();

    // Run the parser on file F.
    // Return 0 on success.
    int parse(const std::string &f);

    // Error handling.
    void error(const nls::location &l, const std::string &m);

    void error(const std::string &m);

    std::map<std::string, int> variables;

    int result;

    // Whether scanner traces should be generated.
    bool trace_scanning;

    // Whether parser traces should be generated.
    bool trace_parsing;

    // The name of the file being parsed.
    // Used later to pass the file name to the location tracker.
    std::string file;
};

}

#endif // ! NIELS_DRIVER_HH
