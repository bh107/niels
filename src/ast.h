#include <cinttypes>
#include <cstddef>
#include <sstream>
#include <iostream>

namespace nir {

// Structs for ranges, and the like
struct Range {
    int64_t begin;
    int64_t end;
    bool begin_inclusive;
    bool end_inclusive;
};
typedef struct Range Range;

// Representation
enum Operator {
    NOOP,
    ADD,
    SUB,
    MUL,
    POW,
    DIV,
    NEG,
    INV,
    QUERY,
    ASSIGN,
    ALIAS
};
typedef enum Operator Operator;

std::string Operator_text(Operator op);

enum Structure {
    NOSTRUCT,
    SHAPE,
    RANGE
};
typedef enum Structure Structure;

std::string Structure_text(Structure structure);

enum Vclass {
    NOVAL,
    I32,
    I64,
    R32,
    R64,
    STRING,
    BOOL,
    IDENT
};
typedef enum Vclass Vclass;

std::string Vclass_text(Vclass vclass);

union Vtype {
    int32_t i32;
    int64_t i64;
    float r32;
    double r64;
    bool boolean;
    char* str;
};
typedef union Vtype Vtype;

std::string Vtype_text(Vclass vclass, Vtype vtype);

class Node {
public:
    Node(void);         // NOOP
    Node(Vclass vclass, int32_t val);  // Constructors for literals
    Node(Vclass vclass, int64_t val);
    Node(Vclass vclass, float val);
    Node(Vclass vclass, double val);
    Node(Vclass vclass, bool val);
    Node(Vclass vclass, char* val);   // strings and idents

    Node(Operator op, Node* left);               // Expressions with unary operator
    Node(Operator op, Node* left, Node* right);  // Expressions with unary operator

    // Fix constructors for structures

    Node* left(void);
    Node* right(void);

    std::string dot(void);
    std::string text(void);
private:
    Operator _operator;
    Structure _structure;
    
    Vclass _vclass;
    Vtype _vtype;

    Node* _left;
    Node* _right;
};

void eval(Node* node);
std::string dot(Node* node);

}
