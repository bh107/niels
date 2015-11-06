#!/usr/bin/env python
import pprint
import itertools

def unary_sigs(vtypes):
    sigs = set()
    for c, res_vtype in enumerate(vtypes):
        (res_t, res_t_enum) = res_vtype
        for in_vtype in vtypes[0:c+1]:
            (in_t, in_t_enum) = in_vtype
            sigs.add((res_t, in_t))
    return sorted(list(sigs))

def binary_sigs(vtypes):
    sigs = set()
    for c, res_vtype in enumerate(vtypes):
        (res_t, res_t_enum) = res_vtype
        for in_vtype in vtypes[0:c+1]:
            (in_t, in_t_enum) = in_vtype
            sigs.add((res_t, res_t, in_t))
            sigs.add((res_t, in_t, res_t))
    return sorted(list(sigs))

def unary_logic_sigs(vtypes):
    return [("bool", in1) for in1, in itertools.product(
        [vtype for vtype, vtype_enum in vtypes],
        repeat=1
    )]

def binary_logic_sigs(vtypes):
    return [("bool", in1, in2) for in1, in2 in itertools.product(
        [vtype for vtype, vtype_enum in vtypes],
        repeat=2
    )]

vtypes = [
    ("bool", "BOOL"),
    ("i32",  "INT32"),
    ("i64",  "INT64"),
    ("r32",  "REAL32"),
    ("r64",  "REAL64")
]

operators = {
    "arithmetic": [
        ("add", "in1->value().{in1_t} + in2->value().{in2_t}",      binary_sigs(vtypes)), 
        ("sub", "in1->value().{in1_t} - in2->value().{in2_t}",      binary_sigs(vtypes)),
        ("mul", "in1->value().{in1_t} * in2->value().{in2_t}",      binary_sigs(vtypes)),
        ("mod", "in1->value().{in1_t} % in2->value().{in2_t}",      binary_sigs(vtypes)),
        ("div", "in1->value().{in1_t} / in2->value().{in2_t}",      binary_sigs(vtypes)),
        ("pow", "pow(in1->value().{in1_t}, in1->value().{in2_t})",  binary_sigs(vtypes)),
        ("neg", "~ in1->value().{in1_t}", unary_sigs(vtypes)),
    ],
    "comparison": [
        ("lthan",       "in1->value().{in1_t} < in2->value().{in2_t}",  binary_logic_sigs(vtypes)),
        ("gthan",       "in1->value().{in1_t} > in2->value().{in2_t}",  binary_logic_sigs(vtypes)),
        ("equal",       "in1->value().{in1_t} == in2->value().{in2_t}", binary_logic_sigs(vtypes)),
        ("not_equal",   "in1->value().{in1_t} != in2->value().{in2_t}", binary_logic_sigs(vtypes)),
        ("lthan_equal", "in1->value().{in1_t} <= in2->value().{in2_t}", binary_logic_sigs(vtypes)),
        ("gthan_equal", "in1->value().{in1_t} >= in2->value().{in2_t}", binary_logic_sigs(vtypes))
    ],
    "logical": [
        ("and", "in1->value().{in1_t} && in2->value().{in2_t}", binary_logic_sigs(vtypes)),
        ("or",  "in1->value().{in1_t} || in2->value().{in2_t}", binary_logic_sigs(vtypes)),
        ("xor", "in1->value().{in1_t} || in2->value().{in2_t}", binary_logic_sigs(vtypes)),
        ("not", "!in1->value().{in1_t}",                        unary_logic_sigs(vtypes)),
    ],
    "bitwise": [
        ("bw_lshift", "in1->value().{in1_t} << in2->value().{in2_t}", binary_sigs(vtypes[0:3])),
        ("bw_rshift", "in1->value().{in1_t} >> in2->value().{in2_t}", binary_sigs(vtypes[0:3])),

        ("bw_and",  "in1->value().{in1_t} & in2->value().{in2_t}", binary_sigs(vtypes[0:3])),
        ("bw_or",   "in1->value().{in1_t} | in2->value().{in2_t}", binary_sigs(vtypes[0:3])),
        ("bw_xor",  "in1->value().{in1_t} ^ in2->value().{in2_t}", binary_sigs(vtypes[0:3])),
        ("bw_not",  "~ in1->value().{in1_t}", unary_sigs(vtypes[0:3])),
    ]
}

def inner_declr(name, expr, sig):
    ninput = len(sig)-1
    res_t = sig[0]
    in1_t = sig[1] if ninput > 0 else  None
    in2_t = sig[2] if ninput > 1 else  None

    declr = "void nls_{name}_{res_t}_%s(Node* res, %s)" % (
        "_".join(["{in%d_t}" % i for i in xrange(1, ninput+1)]),
        ", ".join(["Node* in%d" % i for i in xrange(1, ninput+1)])
    )
    return declr.format(name=name, res_t=res_t, in1_t=in1_t, in2_t=in2_t)

def inner_expr(name, expr, sig):

    ninput = len(sig)-1

    res_t = sig[0]
    in1_t = sig[1] if ninput > 0 else  None
    in2_t = sig[2] if ninput > 1 else  None

    body = "res->value().{res_t} = %s" % expr

    return body.format(
        res_t=res_t, 
        in1_t=in1_t, 
        in2_t=in2_t
    )

def inner_func(name, expr, sig):
    return inner_declr(name, expr, sig)+"\n{\n    "+inner_expr(name, expr, sig) +";\n}"

def operator_func(name, expr, sig):

    ninput = len(sig)-1
    declr = "void nls_operator_{name}(Node* res, %s)" % (
        ", ".join(["Node* in%d" % i for i in xrange(1, ninput+1)])
    )
    body = ["\n{{"]
    body.append("    VType res_t = res->vtype();")
    for i in xrange(1, ninput+1):
        body.append("    VType in%d_t = in%d->vtype();" % (i,i))
    body.append("}}")

    return declr + "\n".join(body)

for area in operators:
    for operator in operators[area]:
        (name, expr, sigs) = operator
        for sig in sigs:
            #print name, "_".join(sig)
            print inner_func(name, expr, sig)
            #print dispatch(name, expr, sig)

