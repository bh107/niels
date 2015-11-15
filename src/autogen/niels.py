#!/usr/bin/env python
import itertools
import pprint
import json as json

def nls2json(obj, level=0):
    """A formatter easing the human readability of niels.json."""

    INDENT = 3
    SPACE = " "
    NEWLINE = "\n"

    def pod_iter(subject):
        """Assumes subject is an iter of tuples or list."""
        count = 0
        for e in subject:
            if (isinstance(e, list) or isinstance(e, tuple)) and len(e)>1:
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

def unary_repeat_sigs(vtypes):
    sigs = set()
    for vtype in vtypes:
        sigs.add(tuple([vtype]*2))
    return sorted(list(sigs))

def binary_repeat_sigs(vtypes):
    sigs = set()
    for vtype in vtypes:
        sigs.add(tuple([vtype]*3))
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

vtypes_grouped = {
    "scalar": ["bul", "i32", "i64", "r32", "r64"],
    "complex": ["c64", "c128"],
    "array": ["bul_a", "i32_a", "i64_a", "r32_a", "r64_a", "c64_a", "c128_a"]
}
vtypes = [vtype for k in vtypes_grouped for vtype in vtypes_grouped[k]]

vtype2ctype = {
    "bul": "bool",
    "i32": "int32_t",
    "i64": "int64_t",
    "r32": "float",
    "r64": "double",
    "c64": "std::complex<float>*",
    "c128": "std::complex<double>*",

    "bul_a": "bxx::multi_array<bool>*",
    "i32_a": "bxx::multi_array<int32_t>*",
    "i64_a": "bxx::multi_array<int64_t>*",
    "r32_a": "bxx::multi_array<float>*",
    "r64_a": "bxx::multi_array<double>*",
    "c64_a": "bxx::multi_array<std::complex<float> >*",
    "c128_a": "bxx::multi_array<std::complex<double> >*"
}

nls = {
    "vtypes": vtypes,
    "vtypes_grouped": vtypes_grouped,

    "vtype2enum":   {vtype: enumize(vtype) for vtype in vtypes},
    "vtype2ctype":  vtype2ctype,
    "vtype2ast":    {vtype: camelize(vtype) for vtype in vtypes},

    "operators": [
        ("arithmetic", "add", 2, {
            "scalar":   (
                "res->value().{res_t} = in1->value().{in1_t} + in2->value().{in2_t}",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "array":    (
                "*res->value().{res_t} = bh_add(*in1->value().{in1_t}, *in2->value().{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"])
            ),
            "complex":  ("", []),
        }), 
        ("arithmetic", "sub", 2, {
            "scalar":   (
                "res->value().{res_t} = in1->value().{in1_t} - in2->value().{in2_t}",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "array":    (
                "*res->value().{res_t} = bxx::bh_subtract(*in1->value().{in1_t}, *in2->value().{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"])
            ),
            "complex":  ("", []),
        }),
        ("arithmetic", "mul", 2, {
            "scalar":   (
                "res->value().{res_t} = in1->value().{in1_t} * in2->value().{in2_t}",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "array":    (
                "*res->value().{res_t} = bxx::bh_multiply(*in1->value().{in1_t}, *in2->value().{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"])
            ),
            "complex":  ("", []),

        }),
        ("arithmetic", "mod", 2, {
            "scalar": (
                "res->value().{res_t} = in1->value().{in1_t} % in2->value().{in2_t}", binary_sigs(vtypes_grouped["scalar"][0:3])
                ),
            "array":    (
                "*res->value().{res_t} = bxx::bh_mod(*in1->value().{in1_t}, *in2->value().{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3])
            ),
            "complex":  ("", []),
        }),
        ("arithmetic", "div", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} / in2->value().{in2_t}", binary_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("arithmetic", "pow", 2, {
            "scalar":   ("res->value().{res_t} = pow(in1->value().{in1_t}, in2->value().{in2_t})",  binary_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),

        ("comparison", "lthan", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} < in2->value().{in2_t}",  binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("comparison", "gthan", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} > in2->value().{in2_t}",  binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("comparison", "equal", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} == in2->value().{in2_t}", binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("comparison", "not_equal", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} != in2->value().{in2_t}", binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("comparison", "lthan_equal", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} <= in2->value().{in2_t}", binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("comparison", "gthan_equal", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} >= in2->value().{in2_t}", binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),

        ("logical", "and", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} && in2->value().{in2_t}", binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("logical", "or", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} || in2->value().{in2_t}", binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("logical", "xor", 2, {
            "scalar":   ("res->value().{res_t} = in1->value().{in1_t} || in2->value().{in2_t}", binary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("logical", "not", 1, {
            "scalar":   ("res->value().{res_t} = !in1->value().{in1_t}", unary_logic_sigs(vtypes_grouped["scalar"])),
            "array":    ("", []),
            "complex":  ("", [])
        }),

        ("bitwise", "bw_lshift", 2, {
            "scalar": ("res->value().{res_t} = in1->value().{in1_t} << in2->value().{in2_t}", binary_sigs(vtypes_grouped["scalar"][0:3])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("bitwise", "bw_rshift", 2, {
            "scalar": ("res->value().{res_t} = in1->value().{in1_t} >> in2->value().{in2_t}", binary_sigs(vtypes_grouped["scalar"][0:3])),
            "array":    ("", []),
            "complex":  ("", [])
        }),

        ("bitwise", "bw_and", 2, {
            "scalar": ("res->value().{res_t} = in1->value().{in1_t} & in2->value().{in2_t}", binary_sigs(vtypes_grouped["scalar"][0:3])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("bitwise", "bw_or", 2, {
            "scalar": ("res->value().{res_t} = in1->value().{in1_t} | in2->value().{in2_t}", binary_sigs(vtypes_grouped["scalar"][0:3])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("bitwise", "bw_xor", 2, {
            "scalar": ("res->value().{res_t} = in1->value().{in1_t} ^ in2->value().{in2_t}", binary_sigs(vtypes_grouped["scalar"][0:3])),
            "array":    ("", []),
            "complex":  ("", [])
        }),
        ("bitwise", "bw_not", 1, {
            "scalar": ("res->value().{res_t} = ~ in1->value().{in1_t}", unary_sigs(vtypes_grouped["scalar"][0:3])),
            "array":    ("", []),
            "complex":  ("", [])
        })
    ]
}

op2node = {}
for k, name, ninput, exprs in nls["operators"]:
    op2node[name] = camelize(name)
nls["op2node"] = op2node

if __name__ == "__main__":
    print nls2json(nls)
