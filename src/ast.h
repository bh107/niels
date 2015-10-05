#include <cinttypes>
#include <cstddef>
#include <sstream>
#include <iostream>

struct Range {
    int64_t begin;
    int64_t end;
    bool begin_inclusive;
    bool end_inclusive;
};
typedef struct Range Range;

union Primitives {
    int32_t int32;
    int64_t int64;
    float real32;
    double real64;
    bool boolean;
    char* str;
};
typedef union PrimitiveType PrimitiveType;

enum SymbolType {
    LITERAL,
    VARIABLE,
    FUNCTION
};
typedef enum SymbolType Symboltype;

struct Symbol {
    SymbolType type;
    Primitives val;
};
typedef struct Symbol Symbol;

class Node {
public:
    Node(void);

    std::string text(void);

    void left(Node* left);
    void right(Node* right);

    Node* left(void);
    Node* right(void);
    Symbol* symbol(void);

    Symbol _symbol;
private:
    Node* _left;
    Node* _right;
};

void eval(Node* node);
