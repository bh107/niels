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

def binary_repeat_sigs(vtypes, layout="aaa"):
    sigs = set()
    if layout == "aaa" or layout == "kkk":
        for vtype in vtypes:
            sigs.add((vtype, vtype, vtype))
    elif layout == "aak":
         for vtype in vtypes:
            sigs.add((vtype, vtype, vtype.replace("_a","")))
    elif layout == "aka":
         for vtype in vtypes:
            sigs.add((vtype, vtype.replace("_a",""), vtype))

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
    "array": ["bul_a", "i32_a", "i64_a", "r32_a", "r64_a"]
}
vtypes = [vtype for k in vtypes_grouped for vtype in vtypes_grouped[k]]

vtype2ctype = {
    "bul": "bool",
    "i32": "int32_t",
    "i64": "int64_t",
    "r32": "float",
    "r64": "double",

    "bul_a": "bxx::multi_array<bool>*",
    "i32_a": "bxx::multi_array<int32_t>*",
    "i64_a": "bxx::multi_array<int64_t>*",
    "r32_a": "bxx::multi_array<float>*",
    "r64_a": "bxx::multi_array<double>*",
}

nls = {
    "vtypes": vtypes,
    "vtypes_grouped": vtypes_grouped,

    "vtype2enum":   {vtype: enumize(vtype) for vtype in vtypes},
    "vtype2ctype":  vtype2ctype,
    "vtype2ast":    {vtype: camelize(vtype) for vtype in vtypes},

    "operators": [
        ("arithmetic", "add", 2, {
            "kkk":   (
                "res.value.{res_t} = in1->value.{in1_t} + in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "aak": (
                "*res.value.{res_t} = bh_add(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bh_add(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bh_add(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aaa")
            )
        }),

        ("arithmetic", "sub", 2, {
            "kkk": (
                "res.value.{res_t} = in1->value.{in1_t} - in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "aak": (
                "*res.value.{res_t} = bh_subtract(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bh_subtract(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bh_subtract(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aaa")
            )
        }),

        ("arithmetic", "mul", 2, {
            "kkk":   (
                "res.value.{res_t} = in1->value.{in1_t} * in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "aak": (
                "*res.value.{res_t} = bh_multiply(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bh_multiply(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bh_multiply(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"], "aaa")
            )
        }),

        ("arithmetic", "mod", 2, {
            "kkk": (
                "res.value.{res_t} = in1->value.{in1_t} % in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"][0:3])
            ),
            "aak": (
                "*res.value.{res_t} = bh_mod(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bh_mod(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bh_mod(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")
            )
        }),

        ("arithmetic", "div", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} / in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "aak": (
                "*res.value.{res_t} = bh_divide(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bh_divide(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bh_divide(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")
            )
        }),

        ("arithmetic", "pow", 2, {
            "kkk":   ("res.value.{res_t} = pow(in1->value.{in1_t}, in2->value.{in2_t})",
                binary_sigs(vtypes_grouped["scalar"])
            ),
            "aak": (
                "*res.value.{res_t} = bh_power(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bh_power(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bh_power(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:], "aaa")
            )
        }),

        ("comparison", "lthan", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} < in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_less(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_less(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_less(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")]
            )
        }),

        ("comparison", "gthan", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} > in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_greater(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_greater(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_greater(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")]
            )
        }),

        ("comparison", "equal", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} == in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_equal(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_equal(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_equal(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")]
            )
        }),

        ("comparison", "not_equal", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} != in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_not_equal(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_not_equal(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_not_equal(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")]
            )
        }),

        ("comparison", "lthan_equal", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} <= in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_less_equal(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_less_equal(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_less_equal(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")]
            )
        }),

        ("comparison", "gthan_equal", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} >= in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_greater_equal(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_greater_equal(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_greater_equal(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", in1, in2) for out, in1, in2 in binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")]
            )
        }),

        ("logical", "and", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} && in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_logical_and(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", "bul_a", "bul")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_logical_and(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", "bul", "bul_a")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_logical_and(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", "bul_a", "bul_a")]
            )
        }),
        
        ("logical", "or", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} || in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_logical_or(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", "bul_a", "bul")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_logical_or(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", "bul", "bul_a")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_logical_or(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", "bul_a", "bul_a")]
            )
        }),

        ("logical", "xor", 2, {
            "kkk":   ("res.value.{res_t} = in1->value.{in1_t} || in2->value.{in2_t}",
                binary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aak":    (
                "*res.value.{res_t} = bxx::bh_logical_xor(*in1->value.{in1_t}, in2->value.{in2_t})",
                [("bul_a", "bul_a", "bul")]
            ),
            "aka":    (
                "*res.value.{res_t} = bxx::bh_logical_xor(in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", "bul", "bul_a")]
            ),
            "aaa":    (
                "*res.value.{res_t} = bxx::bh_logical_xor(*in1->value.{in1_t}, *in2->value.{in2_t})",
                [("bul_a", "bul_a", "bul_a")]
            )
        }),

        ("logical", "not", 1, {
            "kk": (
                "res.value.{res_t} = !in1->value.{in1_t}",
                unary_logic_sigs(vtypes_grouped["scalar"])
            ),
            "aa": (
                "*res.value.{res_t} = bxx::bh_logical_not(*in1->value.{in1_t})",
                [("bul_a", "bul_a")]
            )
        }),

        ("bitwise", "bw_lshift", 2, {
            "kkk": (
                "res.value.{res_t} = in1->value.{in1_t} << in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"][1:3])
            ),
            "aak": (
                "*res.value.{res_t} = bxx::left_shift(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bxx::left_shift(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bxx::left_shift(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")
            ),
        }),

        ("bitwise", "bw_rshift", 2, {
            "kkk": (
                "res.value.{res_t} = in1->value.{in1_t} >> in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"][1:3])
            ),
            "aak": (
                "*res.value.{res_t} = bxx::bh_right_shift(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bxx::bh_right_shift(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bxx::bh_right_shift(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")
            ),
        }),

        ("bitwise", "bw_and", 2, {
            "kkk": (
                "res.value.{res_t} = in1->value.{in1_t} & in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"][1:3])
            ),
            "aak": (
                "*res.value.{res_t} = bxx::bh_bitwise_and(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bxx::bh_bitwise_and(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bxx::bh_bitwise_and(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")
            ),
        }),

        ("bitwise", "bw_or", 2, {
            "kkk": (
                "res.value.{res_t} = in1->value.{in1_t} | in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"][1:3])
            ),
            "aak": (
                "*res.value.{res_t} = bxx::bh_bitwise_or(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bxx::bh_bitwise_or(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bxx::bh_bitwise_or(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")
            ),
        }),

        ("bitwise", "bw_xor", 2, {
            "kkk": (
                "res.value.{res_t} = in1->value.{in1_t} ^ in2->value.{in2_t}",
                binary_sigs(vtypes_grouped["scalar"][1:3])
            ),
            "aak": (
                "*res.value.{res_t} = bxx::bh_bitwise_xor(*in1->value.{in1_t}, in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aak")
            ),
            "aka": (
                "*res.value.{res_t} = bxx::bh_bitwise_xor(in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aka")
            ),
            "aaa": (
                "*res.value.{res_t} = bxx::bh_bitwise_xor(*in1->value.{in1_t}, *in2->value.{in2_t})",
                binary_repeat_sigs(vtypes_grouped["array"][1:3], "aaa")
            ),
        }),

        ("bitwise", "bw_not", 1, {
            "kk": (
                "res.value.{res_t} = ~ in1->value.{in1_t}",
                unary_sigs(vtypes_grouped["scalar"][1:3])
            ),
            "aa": (
                "*res.value.{res_t} = bxx::bh_invert(*in1->value.{in1_t})",
                unary_repeat_sigs(vtypes_grouped["array"][1:3])
            ),
        })
    ]
}

op2node = {}
for k, name, ninput, exprs in nls["operators"]:
    op2node[name] = camelize(name)
nls["op2node"] = op2node

if __name__ == "__main__":
    print nls2json(nls)
