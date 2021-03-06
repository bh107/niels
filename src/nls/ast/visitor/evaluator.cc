#include <iostream>
#include <nls/ast/exception.hh>
#include <nls/ast/visitor/evaluator.hh>
#include <nls/utils.hh>

using namespace std;

namespace nls {
namespace ast {

// Constructor
Evaluator::Evaluator(void)
    : Visitor()
{
}
Evaluator::Evaluator(SymbolTable& symbolTable)
    : Visitor(), _symbolTable(symbolTable)
{
}

Evaluator::~Evaluator(void)
{
}

// Stack operations
Variant Evaluator::peek(void)
{
    return _stack.top();
}

Variant Evaluator::pop(void)
{
    Variant res = _stack.top();
    _stack.pop();
    return res;
}

void Evaluator::push(Variant var)
{
    _stack.push(var);
}

//
// I wouldt call it garbage collection... but it's something...
// Atleast this in entry-point/hook for cleaning up stuff
//
void Evaluator::garbage(Variant& val)
{
    switch(val.value_type) {
    case NLS_UND: break;
    case NLS_BUL: break;
    case NLS_I32: break;
    case NLS_I64: break;
    case NLS_R32: break;
    case NLS_R64: break;
    case NLS_C64: delete val.value.c64; break;
    case NLS_C128: delete val.value.c128; break;

    case NLS_BUL_A: delete val.value.bul_a; break;
    case NLS_I32_A: delete val.value.i32_a; break;
    case NLS_I64_A: delete val.value.i64_a; break;
    case NLS_R32_A: delete val.value.r32_a; break;
    case NLS_R64_A: delete val.value.r64_a; break;
    case NLS_C64_A: delete val.value.c64_a; break;
    case NLS_C128_A: delete val.value.c128_a; break;
    case NLS_STR: break;

    case NLS_FUN: break;
    case NLS_REC: break;
    }
}

// Visitors

/*
    post-order

    pops:
     - val
*/
void Evaluator::visit(Query& node)
{
    walk(node);

    Variant val = pop();
    cout << variant_text(val) << endl;

    garbage(val);
}

//
// Array Construction
//

/*
    in-order

    Pops
        - rank

    Pushes
        - shape
        - rank
*/
void Evaluator::visit(Shape& node)
{
    Variant rank = pop();               // Pop the rank and increment it
    rank.value.i64++;

    node.left()->accept(*this);         // Evaluate shape
    Variant shape = pop();              // Pop shape

    if (shape.value_type != NLS_I64) {  // Check the shape
        cout << "Invalid shape value." << endl;
    }

    push(shape);                        // Push shape
    push(rank);                         // Push rank

    if (node.right()) {
        node.right()->accept(*this);
    }
}

/*
    post-order

    pops
     - NOTHING

    pushes
     - array
*/
void Evaluator::visit(As& node)
{
    node.left()->accept(*this);             // Evaluate what we want to turn into an array
    
    Variant in1 = pop();                    // Pop the thing of the stack

    push({0, NLS_I64});                     // Push rank(0)
    node.right()->accept(*this);            // Evaluate shape subtree

    // TODO: Check that the values on stack are valid evaluation of AS

    const uint64_t rank = pop().value.i64;  // Pop rank
    
    int64_t shape[16];                      // Pop shapes
    for(uint64_t rank_i=0; rank_i<rank; ++rank_i) {
        Variant val = pop();
        shape[rank_i] = val.value.i64;
    }

    Variant res;                            // Container for array

    switch(in1.value_type) {                // Construct the array and generator expression
    case NLS_BUL:
        res.value_type = NLS_BUL_A;
        res.value.bul_a = new bul_a_type((const uint64_t)rank, (const int64_t*)shape);
        res.value.bul_a->link();
        *(res.value.bul_a) = in1.value.bul;
        break;
    case NLS_I32:
        res.value_type = NLS_I32_A;
        res.value.i32_a = new i32_a_type((const uint64_t)rank, (const int64_t*)shape);
        res.value.i32_a->link();
        *(res.value.i32_a) = in1.value.i32;
        break;
    case NLS_I64:
        res.value_type = NLS_I64_A;
        res.value.i64_a = new i64_a_type((const uint64_t)rank, (const int64_t*)shape);
        res.value.i64_a->link();
        *(res.value.i64_a) = in1.value.i64;
        break;
    case NLS_R32:
        res.value_type = NLS_R32_A;
        res.value.r32_a = new r32_a_type((const uint64_t)rank, (const int64_t*)shape);
        res.value.r32_a->link();
        *(res.value.r32_a) = in1.value.r32;
        break;
    case NLS_R64:
        res.value_type = NLS_R64_A;
        res.value.r64_a = new r64_a_type((const uint64_t)rank, (const int64_t*)shape);
        res.value.r64_a->link();
        *(res.value.r64_a) = in1.value.r64;
        break;
    default:
        cout << "ERROR As, unsupported array construction input." << endl;
    }

    push(res);                              // Push the array onto the stack
}

void Evaluator::visit(While& node)
{
    node.left()->accept(*this);             // Eval condition
    Variant cond = pop();                   // Pop the result
    
    bool cont = true;
    while(cond.value.bul && cont) {
        // TODO: Push scope
        try {
            node.right()->accept(*this);    // Evaluate the loop body
        } catch (escape_flow& e) {
            cont = pop().value.bul;         // Get the continue result
        }
        // TODO: Pop scope

        node.left()->accept(*this);         // Re-evaluate condition
        cond = pop();                       // Pop the result
    }
}

void Evaluator::visit(Continue& node)
{
    push({true, NLS_BUL});
    throw escape_flow();
}

void Evaluator::visit(Break& node)
{
    push({false, NLS_BUL});
    throw escape_flow();
}

void Evaluator::visit(Return& node)
{
    walk(node);
    throw escape_flow();
}

/*
    Resolve ident in symbol table.
    If it does not exist then add an NLS_UND Variant.
*/
void Evaluator::visit(Ident& node)
{
    Variant var = _symbolTable.get(node.name());

    Variant res = var;          // Copy the value in symbol table

    switch(var.value_type) {    // Construct views of arrays
    case NLS_BUL_A:
        res.value.bul_a = new bxx::multi_array<bool>();
        *res.value.bul_a = *var.value.bul_a;
        break;
    case NLS_I32_A:
        res.value.i32_a = new bxx::multi_array<int32_t>();
        *res.value.i32_a = *var.value.i32_a;
        break;
    case NLS_I64_A:
        res.value.i64_a = new bxx::multi_array<int64_t>();
        *res.value.i64_a = *var.value.i64_a;
        break;
    case NLS_R32_A:
        res.value.r32_a = new bxx::multi_array<float>();
        *res.value.r32_a = *var.value.r32_a;
        break;
    case NLS_R64_A:
        res.value.r64_a = new bxx::multi_array<double>();
        *res.value.r64_a = *var.value.r64_a;
        break;
    }
    push(res);
}

// Assignment
void Evaluator::visit(Assign& node)
{
    walk(node);

    Variant expr = pop();
    Variant var = pop();

    switch(var.value_type) {
    case NLS_UND:
    case NLS_BUL:
    case NLS_I32:
    case NLS_I64:
    case NLS_R32:
    case NLS_R64:
        var = expr;
        _symbolTable.put(node.left()->name(), var);
        break;

    case NLS_BUL_A:
        (*var.value.bul_a)(*expr.value.bul_a);
        delete var.value.bul_a;
        delete expr.value.bul_a;
        break;
    case NLS_I32_A:
        (*var.value.i32_a)(*expr.value.i32_a);
        delete var.value.i32_a;
        delete expr.value.i32_a;
        break;
    case NLS_I64_A:
        (*var.value.i64_a)(*expr.value.i64_a);
        delete var.value.i64_a;
        delete expr.value.i64_a;
        break;
    case NLS_R32_A:
        (*var.value.r32_a)(*expr.value.r32_a);
        delete var.value.r32_a;
        delete expr.value.r32_a;
        break;
    case NLS_R64_A:
        (*var.value.r64_a)(*expr.value.r64_a);
        delete var.value.r64_a;
        delete expr.value.r64_a;
        break;
    }

}

void Evaluator::visit(When& node)
{
    node.left()->accept(*this);         // Pushes expression value on stack
    try {
        node.right()->accept(*this);    // Go down until breaking out
    } catch (escape_flow& e) {
        Variant comp = pop();           // Pop the condition var
        garbage(comp);
    }
}

void Evaluator::visit(CaseList& node)
{
    walk(node);
}

void Evaluator::visit(Is& node)
{
    Variant comp = peek();              // Get the when-expr-value

    node.left()->accept(*this);         // Evaluate is-expr
    Variant expr = pop();               // Get the is-expr-value

    if (equivalent(comp, expr)) {       // Compare is-expr-value and when-expr-value
        node.right()->accept(*this);    // Evaluate is-body
        throw escape_flow();            // Escape recursion
    }
}

void Evaluator::visit(Otherwise& node)
{
    node.right()->accept(*this);        // Tail, always execute regardless of value
    throw escape_flow();                // Escape recusion

    // NOTE: The exception is thrown to normalize the handling when
    //       visiting "When"
}


}}
