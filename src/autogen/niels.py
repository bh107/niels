#!/usr/bin/env python
import itertools
import pprint
import json as json
import yaml

def nls2json(obj, level=0):
    """A formatter easing the human readability of niels.json."""

    INDENT = 3
    SPACE = " "
    NEWLINE = "\n"

    def pod_iter(subject):
        """Assumes subject is an iter of tuples or list."""
        count = 0
        for e in subject:
            if isinstance(e, list) or isinstance(e, tuple):
                count += 1
        return count == 0

    ret = ""
    if isinstance(obj, dict):
        ret += "{" + NEWLINE
        comma = ""
        for k, v in sorted(obj.items()):
            ret += comma
            comma = ",\n"
            ret += SPACE * INDENT * (level+1)
            ret += '"' + str(k) + '":' + SPACE
            ret += nls2json(v, level + 1)

        ret += NEWLINE + SPACE * INDENT * level + "}"
    elif isinstance(obj, basestring):
        ret += '"' + obj + '"'
    elif isinstance(obj, list) and pod_iter(obj):
        ret += "[" + ", ".join([nls2json(e, level+1) for e in obj]) + "]"
    elif isinstance(obj, list):
        ret += "[" + ", ".join([
            NEWLINE + SPACE * INDENT * level + nls2json(e, level+1) for e in obj
        ]) + "]"
    elif isinstance(obj, tuple) and pod_iter(obj):
        ret += "[" + ", ".join([nls2json(e, level+1) for e in obj]) + "]"
    elif isinstance(obj, tuple):
        ret += "[" + ", ".join([
            NEWLINE + SPACE * INDENT * level + nls2json(e, level+1) for e in obj
        ]) + "]"
    elif isinstance(obj, bool):
        ret += "true" if obj else "false"
    elif isinstance(obj, int):
        ret += str(obj)
    elif isinstance(obj, float):
        ret += '%.7g' % obj
    else:
        raise TypeError("Unknown type '%s' for json serialization" % str(type(obj)))
    return ret

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
    """Returns Node names such as i64 -> R64 and less_than -> LessThan."""
    return "".join([w.lower().capitalize() for w in text.split("_")])

def enumize(text):
    """Returns ENUM names from vtype. E.g. i64 -> NLS_I64."""
    return "NLS_%s" % text.upper()

"""
vtypes = ["bul", "i32", "i64", "r32", "r64", "bul_a", "i32_a", "i64_a", "r32_a", "r64_a"]
ctypes = ["bool", "int32_t", "int64_t", "float", "double", "bxx::multi_array<bool>", "bxx::multi_array<int32_t>", "bxx::multi_array<int64_t>", "bxx::multi_array<float>", "bxx::multi_array<double>"]
"""

vtypes = ["bul", "i32", "i64", "r32", "r64"]
ctypes = ["bool", "int32_t", "int64_t", "float", "double"]

nls = {
    "vtypes": vtypes,

    "vtype2enum": {vtype: enumize(vtype) for vtype in vtypes},
    "vtype2ctype": {vtype: ctype for vtype, ctype in zip(vtypes, ctypes)},
    "vtype2ast": {vtype: camelize(vtype) for vtype in vtypes},

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
    print nls2json(nls)
