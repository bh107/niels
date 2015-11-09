#!/usr/bin/env python
import itertools
import pprint
import json

def unary_sigs(vtypes):
    sigs = set()
    for c, res_vtype in enumerate(vtypes):
        for in_vtype in vtypes[0:c+1]:
            sigs.add((res_vtype, in_vtype))
    return sorted(list(sigs))

def binary_sigs(vtypes):
    sigs = set()
    for c, res_vtype in enumerate(vtypes):
        for in_vtype in vtypes[0:c+1]:
            sigs.add((res_vtype, res_vtype, in_vtype))
            sigs.add((res_vtype, in_vtype, res_vtype))
    return sorted(list(sigs))

def unary_logic_sigs(vtypes):
    return [("bul", in1) for in1, in itertools.product(vtypes, repeat=1)]

def binary_logic_sigs(vtypes):
    return [("bul", in1, in2) for in1, in2 in itertools.product(vtypes, repeat=2)]

def camelize(text):
    return "".join([w.lower().capitalize() for w in text.split("_")])

def enumize(text):
    return "NLS_%s" % text.upper()

def podify(vtype, ctype):
    return (vtype, enumize(vtype), ctype, camelize(vtype))

vtypes = ["bul", "i32", "i64", "r32", "r64"]
ctypes = ["bool", "int32_t", "int64_t", "float", "double"]

nls_pods = [podify(vtype, ctype) for vtype, ctype in zip(vtypes, ctypes)]

vtype2enum  = {vtype: vtype_enum  for vtype, vtype_enum, vtype_ctype, vtype_node in nls_pods}
vtype2ctype = {vtype: vtype_ctype for vtype, vtype_enum, vtype_ctype, vtype_node in nls_pods}

nls = {
    "vtypes": nls_pods,

    "operators": {
        "arithmetic": [
            ("add", "in1->value().{in1_t} + in2->value().{in2_t}",      binary_sigs(vtypes)), 
            ("sub", "in1->value().{in1_t} - in2->value().{in2_t}",      binary_sigs(vtypes)),
            ("mul", "in1->value().{in1_t} * in2->value().{in2_t}",      binary_sigs(vtypes)),
            ("mod", "in1->value().{in1_t} % in2->value().{in2_t}",      binary_sigs(vtypes[0:3])),
            ("div", "in1->value().{in1_t} / in2->value().{in2_t}",      binary_sigs(vtypes)),
            ("pow", "pow(in1->value().{in1_t}, in2->value().{in2_t})",  binary_sigs(vtypes)),
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
}

if __name__ == "__main__":
    print json.dumps(nls, indent=2, separators=(", ", ": "))
