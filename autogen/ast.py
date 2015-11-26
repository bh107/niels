#!/usr/bin/env python
import itertools
import argparse
import pprint
import glob
import yaml
import os

from mako.template import Template
from niels import camelize

def indent(lines, spaces=4):
    """
    Indent the given string of lines with "spaces" " " and
    strip trailing newline.
    """
    indented = [" "*spaces+line.strip() for line in lines.split("\n")]
    return ("\n".join(indented)).rstrip()


def fill_ast(ast):
    """
    Fills out the ast-dict with default values.
    """

    if "guard" not in ast["namespace"]:
        ast["namespace"]["guard"] = ast["namespace"]["name"].upper()

    for node in ast["nodes"]:
        if "guard" not in node:
            node["guard"] = node["name"].upper()

        if "shape" not in node:
            node["shape"] = "box"

        if "class" not in node:
            node["class"] = camelize(node["name"])

        if "constr_lval" in node and node["constr_lval"]:
            node["constr_lval"] = indent(node["constr_lval"])
        else:
            node["constr_lval"] = None

    ast["nodes"].sort() # Sorting the nodes by name
                        # Reasoning: the default listing in filesystems
                        #            wont preserve the order as given in
                        #            in ast.yaml
                        # Using a probably similar sort/arrangement of
                        # names when looking in dir-listing and in
                        # visits_auto etc. might be convenient
                        # it might also be really annoying... who knows?

def gen_nodes(ast, output_root):
    """
    Generates code for and stores to file:

      * output_root/ast/add.hh
      * output_root/ast/add.cc
      * output_root/ast/sub.hh
      * output_root/ast/sub.cc
      * ...

    @returns list of generated files.
    """

    generated = []  # Names of generated files

    hdr_template = os.sep.join(["templates", "nodes_hh.tpl"])
    src_template = os.sep.join(["templates", "nodes_cc.tpl"])

    namespace = ast["namespace"]
    for node in ast["nodes"]:
        # Fill out the HEADER template
        hdr = Template(filename=hdr_template).render(
            namespace=namespace,
            node=node
        )
        # Write it to file
        hdr_path = output_root +os.sep+ "%s.hh" % node["name"]
        with open(hdr_path, "w") as fd:
            fd.write(hdr)
        # Log the generated file
        generated.append(hdr_path)

        # Fill out the SOURCE template
        src = Template(filename=src_template).render(
            namespace=namespace,
            node=node
        )
        src_path = output_root +os.sep+ "%s.cc" % node["name"]
        with open(src_path, "w") as fd:
            fd.write(src)
        # Log the generated file
        generated.append(src_path)

    return generated

def gen_visitor_visit_auto_hh_inc(ast, output_root):

    code = Template(filename=os.sep.join([
        "templates",
        "visitor_visit_auto_hh_inc.tpl"
    ])).render(namespace=ast["namespace"], nodes=ast["nodes"])

    inc_path = output_root +os.sep+ "visitor" +os.sep+ "visitor_visit_auto_hh.inc"
    with open(inc_path, "w") as fd:
        fd.write(code)

    return [inc_path]

def gen_evaluator_visit_auto_hh_inc(ast, output_root):
   
    code = Template(filename=os.sep.join([
        "templates",
        "evaluator_visit_auto_hh_inc.tpl"
    ])).render(namespace=ast["namespace"], nodes=ast["nodes"])

    inc_path = output_root +os.sep+ "visitor" +os.sep+ "evaluator_visit_auto_hh.inc"
    with open(inc_path, "w") as fd:
        fd.write(code)

    return [inc_path]

def main(args):
    ast = yaml.load(open(args.yaml))    # Load yaml
    fill_ast(ast)                       # Fill it out with default values

    output_root = os.sep.join(["out", ast["namespace"]["name"], "ast"])

    generated_nodes = gen_nodes(ast, output_root)
    gen_visitor_visit_auto_hh_inc(ast, output_root)
    gen_evaluator_visit_auto_hh_inc(ast, output_root)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Autogenerate code based on ast.yaml.'
    )
    parser.add_argument(
        "--yaml",
        type=str,
        help="Path to ast.yaml",
        default=os.sep.join([os.path.dirname(os.path.realpath(__file__)), "ast.yaml"])
    )

    args = parser.parse_args()          # Parse command-line arguments
    main(args)                          # Run main
