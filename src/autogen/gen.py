#!/usr/bin/env python
import itertools
import argparse
import pprint
import glob
import json
import os

from mako.template import Template
from niels import camelize

def cmake(autogen_path):
    """
    Create OUTPUT and DEPENDS strings for cmake custom target.
    Run this function manually to update CMakelists.txt
    """

    output = " ".join(["${CMAKE_CURRENT_SOURCE_DIR}/%s" % gen for gen in generators])
    depends = " ".join(["${CMAKE_CURRENT_SOURCE_DIR}/%s/%s"% (path, filename) for path, dirs, files in os.walk(autogen_path) for filename in (fn for fn in files if not fn.startswith("."))])

    return "OUTPUT %s\n\nDEPENDS %s" % (output, depends)

#
# Construct vtype nodes which the scanner can construct.
#
def vtype_hh(nls):
    tmpl = Template(filename=os.sep.join(["templates", "ast_vtype_hh.tpl"]))

    return tmpl.render(
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

def vtype_cc(nls):
    tmpl = Template(filename=os.sep.join(["templates", "ast_vtype_cc.tpl"]))
    return tmpl.render(
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

#
# Construct expr nodes for the grammar to construct
#

def unary_ops_hh(nls):

    ops = [op for op in nls["operators"] if op[2] == 1]
    
    return Template(filename=os.sep.join(["templates", "ast_unary_ops_hh.tpl"])).render(
        operators=ops,
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

def unary_ops_cc(nls):
    ops = [op for op in nls["operators"] if op[2] == 1]

    return Template(filename=os.sep.join(["templates", "ast_unary_ops_cc.tpl"])).render(
        operators=ops,
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

def binary_ops_hh(nls):
    ops = [op for op in nls["operators"] if op[2] == 2]

    return Template(filename=os.sep.join(["templates", "ast_binary_ops_hh.tpl"])).render(
        operators=ops,
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

def binary_ops_cc(nls):
    ops = [op for op in nls["operators"] if op[2] == 2]

    return Template(filename=os.sep.join(["templates", "ast_binary_ops_cc.tpl"])).render(
        operators=ops,
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

generators = {
    "ast_vtype_auto.hh": vtype_hh,
    "ast_vtype_auto.cc": vtype_cc,
    "ast_binary_ops_auto.hh": binary_ops_hh,
    "ast_binary_ops_auto.cc": binary_ops_cc,
    "ast_unary_ops_auto.hh": unary_ops_hh,
    "ast_unary_ops_auto.cc": unary_ops_cc,
}

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
