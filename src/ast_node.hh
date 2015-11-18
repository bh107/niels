#ifndef NIELS_AST_NODE_HH
#define NIELS_AST_NODE_HH
#include <complex>
#include <cinttypes>
#include <ast_node.hh>
#include <bxx/bohrium.hpp>

namespace nls {

class Driver;

typedef bxx::multi_array<bool> bul_a_type;
typedef bxx::multi_array<int32_t> i32_a_type;
typedef bxx::multi_array<int64_t> i64_a_type;
typedef bxx::multi_array<float> r32_a_type;
typedef bxx::multi_array<double> r64_a_type;

union Value {
    bool bul;
    int32_t i32;
    int64_t i64;
    float r32;
    double r64;

    std::complex<float>* c64;
    std::complex<double>* c128;

    bxx::multi_array<bool>* bul_a;
    bxx::multi_array<int32_t>* i32_a;
    bxx::multi_array<int64_t>* i64_a;
    bxx::multi_array<float>* r32_a;
    bxx::multi_array<double>* r64_a;

    bxx::multi_array<std::complex<float> >* c64_a;
    bxx::multi_array<std::complex<double> >* c128_a;

    std::string* str;
};
typedef union Value Value;

enum VType : uint64_t {
    NLS_UND = 0,

    NLS_BUL = 1,
    NLS_I32 = 2,
    NLS_I64 = 4,
    NLS_R32 = 8,
    NLS_R64 = 16,
    NLS_C64 = 32,
    NLS_C128 = 64,

    NLS_BUL_A = 128,
    NLS_I32_A = 256,
    NLS_I64_A = 512,
    NLS_R32_A = 1024,
    NLS_R64_A = 2048,
    NLS_C64_A = 4096,
    NLS_C128_A = 8192,

    NLS_STR = 16384
};
typedef enum VType VType;

#define SCALAR  (NLS_BUL    |NLS_I32    |NLS_I64    |NLS_R32    |NLS_R64    |NLS_C64    |NLS_C128)
#define ARRAY   (NLS_BUL_A  |NLS_I32_A  |NLS_I64_A  |NLS_R32_A  |NLS_R64_A  |NLS_C64_A  |NLS_C128_A)

enum SType : uint32_t {
    UNKNOWN,

    EXPR,
    VAR,
    REC,
    ATTR,
    FUN,
    ARG,
    PARAM,
    MOD,
    COLL
};
typedef enum SType SType;

class Node {
public:
    Node(void);
    Node(Node* left);
    Node(Node* left, Node* right);

    Node* left(void);
    Node* right(void);
    void left(Node* left);
    void right(Node* right);

    void append(Node* node);

    VType vtype(void);
    void vtype(VType val);

    SType stype(void);
    void stype(SType val);

    void value(Value& val);

    Value& value(void);

    bool defined(void);
    bool known(void);

    std::string& str(void);

    std::string dot(void);
    std::string dot_relation(void);
    virtual std::string txt(void);

    std::string& name(void);
    void name(const std::string& val);

    virtual void eval(Driver& env);

    virtual std::string dot_shape(void);
    virtual std::string dot_label(void);
    virtual std::string dot_color(void);
    
    virtual ~Node(void);

protected:
    SType _stype;
    VType _vtype;
    Value _value;

    Node* _left;
    Node* _right;

    std::string _name;
};

}

#endif

