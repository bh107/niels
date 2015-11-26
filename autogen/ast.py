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

def gen_nodes(ast, output_root, template_root):
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

    hdr_template = os.sep.join([template_root, "node_skeleton_hh.tpl"])
    src_template = os.sep.join([template_root, "node_skeleton_cc.tpl"])

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

def gen_nodes_hh(ast, output_root, template_root):

    code = Template(filename=os.sep.join([
        template_root,
        "nodes_hh.tpl"
    ])).render(namespace=ast["namespace"], nodes=ast["nodes"])

    inc_path = output_root +os.sep+ "nodes.hh"
    with open(inc_path, "w") as fd:
        fd.write(code)

    return [inc_path]

def gen_visitor_visit_auto_hh_inc(ast, output_root, template_root):

    code = Template(filename=os.sep.join([
        template_root,
        "visitor_visit_auto_hh_inc.tpl"
    ])).render(namespace=ast["namespace"], nodes=ast["nodes"])

    inc_path = output_root +os.sep+ "visitor" +os.sep+ "visitor_visit_auto_hh.inc"
    with open(inc_path, "w") as fd:
        fd.write(code)

    return [inc_path]

def gen_evaluator_visit_auto_hh_inc(ast, output_root, template_root):
   
    code = Template(filename=os.sep.join([
        template_root,
        "evaluator_visit_auto_hh_inc.tpl"
    ])).render(namespace=ast["namespace"], nodes=ast["nodes"])

    inc_path = output_root +os.sep+ "visitor" +os.sep+ "evaluator_visit_auto_hh.inc"
    with open(inc_path, "w") as fd:
        fd.write(code)

    return [inc_path]

def main(args):
    
    yaml_path = os.path.expandvars(os.path.expanduser(args.yaml))

    ast = yaml.load(open(yaml_path))    # Load yaml
    fill_ast(ast)                       # Fill it out with default values

    output_root = os.path.expandvars(os.path.expanduser(os.sep.join([
        args.output_root,
        ast["namespace"]["name"],
        "ast"]
    )))
    template_root = os.path.expandvars(os.path.expanduser(args.template_root))

    for path in [output_root, template_root]:        # Check paths
        if not os.path.exists(path):
            print("Path(%s) does not exist." % path)
            return

    print (output_root, template_root)

    generated = []
    generated += gen_nodes(ast, output_root, template_root)     # Nodes
    generated += gen_nodes_hh(ast, output_root, template_root)  # Nodes header
    generated += gen_visitor_visit_auto_hh_inc(                 # Visitor::visit(...)
        ast, output_root, template_root
    )
    generated += gen_evaluator_visit_auto_hh_inc(               # Evaluator::visit(...)
        ast,
        output_root,
        template_root
    )   

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Autogenerate code based on ast.yaml.'
    )
    parser.add_argument(
        "--yaml",
        type=str,
        help="Path to ast.yaml",
        default=os.sep.join([
            os.path.dirname(os.path.realpath(__file__)),
            "ast.yaml"
        ])
    )
    parser.add_argument(
        "--template_root",
        type=str,
        help="Path to dir containing templates.",
        default=os.sep.join([
            os.path.dirname(os.path.realpath(__file__)),
            "templates"
        ])
    )
    parser.add_argument(
        "--output_root",
        type=str,
        help="Path to dir to place generated files.",
        default=os.sep.join([
            os.path.dirname(os.path.realpath(__file__)),
            "out"
        ])
    )
    
    args = parser.parse_args()          # Parse command-line arguments
    main(args)                          # Run main
