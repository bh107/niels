#!/usr/bin/env python
import itertools
import argparse
import pprint
import glob
import json
import os

from mako.template import Template
from niels import camelize, vtype2enum, vtype2ctype

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

def operator_func(name, expr, sigs):

    ninput = len(sigs[0])-1

    declr = "void nls_operator_{name}(Node* res, %s)" % (
        ", ".join(["Node* in%d" % i for i in xrange(1, ninput+1)])
    )
    body = ["\n{{"]
    body.append(" "*4 + "VType res_t = res->vtype();")
    for i in xrange(1, ninput+1):
        body.append(" "*4 + "VType in%d_t = in%d->vtype();" % (i,i))

    if (ninput == 2):
        body.append(" "*4 + "uint64_t mask = (res_t << 32) + (in1_t << 16) + in2_t;")
    elif (ninput == 1):
        body.append(" "*4 + "uint64_t mask = (res_t << 16) + in1_t;")

    body.append(" "*4 +"switch(mask) {{")
    for sig in sigs:
        res_t = sig[0]
        in1_t = sig[1] if ninput > 0 else  None
        in2_t = sig[2] if ninput > 1 else  None

        case = " "*4 + "case "
        if (ninput == 2):
            case += "(%s << 32 ) + (%s << 16) + %s" % (
                vtype2enum[res_t], vtype2enum[in1_t], vtype2enum[in2_t]
            )
        elif (ninput == 1):
            case += "(%s << 16) + %s" % (vtype2enum[res_t], vtype2enum[in1_t])
        case += ":"
        body.append(case)
        body.append(" "*8 +"%s;" % inner_expr(name, expr, sig))
        body.append(" "*8 +"break;")
    body.append(" "*4 + "}}")
    body.append("}}")

    func = declr + "\n".join(body)
    return func.format(name=name)

def vtype_hh(nls):
    tmpl = Template(filename=os.sep.join(["templates", "ast_vtype_hh.tpl"]))
    return tmpl.render(vtypes=nls["vtypes"])

def vtype_cc(nls):
    tmpl = Template(filename=os.sep.join(["templates", "ast_vtype_cc.tpl"]))
    return tmpl.render(vtypes=nls["vtypes"])

def unary_ops_hh(nls):
    ops = [(name, camelize(name), expr, sigs, k) for k in nls["operators"] 
                           for name, expr, sigs in nls["operators"][k] if len(sigs[0])==2]

    return Template(filename=os.sep.join(["templates", "ast_unary_ops_hh.tpl"])).render(
        operators=ops
    )

def unary_ops_cc(nls):
    ops = [(name, camelize(name), expr, sigs, k) for k in nls["operators"] 
                           for name, expr, sigs in nls["operators"][k] if len(sigs[0])==2]

    return Template(filename=os.sep.join(["templates", "ast_unary_ops_cc.tpl"])).render(
        operators=ops,
        vtype2enum=vtype2enum
    )

def binary_ops_hh(nls):
    ops = [(name, camelize(name), expr, sigs, k) for k in nls["operators"] 
                           for name, expr, sigs in nls["operators"][k] if len(sigs[0])==3]

    return Template(filename=os.sep.join(["templates", "ast_binary_ops_hh.tpl"])).render(
        operators=ops,
        vtype2enum=vtype2enum
    )

def binary_ops_cc(nls):
    ops = [(name, camelize(name), expr, sigs, k) for k in nls["operators"] 
                           for name, expr, sigs in nls["operators"][k] if len(sigs[0])==3]

    return Template(filename=os.sep.join(["templates", "ast_binary_ops_cc.tpl"])).render(
        operators=ops,
        vtype2enum=vtype2enum
    )

generators = {
    "ast_vtype_auto.hh": vtype_hh,
    "ast_vtype_auto.cc": vtype_cc,
    "ast_binary_ops_auto.hh": binary_ops_hh,
    "ast_binary_ops_auto.cc": binary_ops_cc,
    "ast_unary_ops_auto.hh": unary_ops_hh,
    "ast_unary_ops_auto.cc": unary_ops_cc,
}

def cmake(autogen_path):
    """Create OUTPUT and DEPENDS strings for cmake custom target."""
    output = " ".join(["${CMAKE_CURRENT_SOURCE_DIR}/%s" % gen for gen in generators])
    depends = " ".join(["${CMAKE_CURRENT_SOURCE_DIR}/%s/%s"% (path, filename) for path, dirs, files in os.walk(autogen_path) for filename in (fn for fn in files if not fn.startswith("."))])

    return "OUTPUT %s\n\nDEPENDS %s" % (output, depends)

def main():
    parser = argparse.ArgumentParser(description='Autogenerate code based on niels.json.')
    parser.add_argument(
        "--json",
        type=str,
        help="Path to niels.json",
        default=os.sep.join([os.path.dirname(os.path.realpath(__file__)), "niels.json"])
    )
    parser.add_argument(
        "--generator",
        type=str,
        help="Generator to run.",
        choices = generators.keys(),
        required=True
    )

    args = parser.parse_args()              # Parse command-line arguments
    nls = json.load(open(args.json))        # Load json
    print(generators[args.generator](nls))  # Call generator

if __name__ == "__main__":
    main()
