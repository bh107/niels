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
# Construct expr nodes for the grammar to construct
#

def evaluator_visit_unary_auto_cc(nls):
    ops = [op for op in nls["operators"] if op[2] == 1]

    return Template(filename=os.sep.join(["templates", "evaluator_visit_unary_auto_cc.tpl"])).render(
        operators=ops,
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

def evaluator_visit_binary_auto_cc(nls):
    ops = [op for op in nls["operators"] if op[2] == 2]

    return Template(filename=os.sep.join(["templates", "evaluator_visit_binary_auto_cc.tpl"])).render(
        operators=ops,
        vtypes=nls["vtypes"],
        vtype2ast=nls["vtype2ast"],
        vtype2enum=nls["vtype2enum"],
        vtype2ctype=nls["vtype2ctype"],
        op2node=nls["op2node"],
    )

generators = {
    "evaluator_visit_variant_auto.cc":  evaluator_visit_variant_auto_cc,
    "evaluator_visit_unary_auto.cc":    evaluator_visit_unary_auto_cc,
    "evaluator_visit_binary_auto.cc":   evaluator_visit_binary_auto_cc,
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
    )
    parser.add_argument(
        "--cmake",
        action='store_true',
        help="Produce CMAKE custom depends."
    )

    args = parser.parse_args()              # Parse command-line arguments
    nls = json.load(open(args.json))        # Load json
    if args.cmake:
        print cmake("templates")
    elif args.generator:
        print(generators[args.generator](nls))  # Call generator
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
