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

        if "evaluator" not in node:
            node["evaluator"] = "default"

    ast["nodes"].sort() # Sorting the nodes by name
                        # Reasoning: the default listing in filesystems
                        #            wont preserve the order as given in
                        #            in ast.yaml
                        # Using a probably similar sort/arrangement of
                        # names when looking in dir-listing and in
                        # visits_auto etc. might be convenient
                        # it might also be really annoying... who knows?


class Emitter(object):

    def __init__(self, output_root, template_root):
        self.output_root = output_root
        self.template_root = template_root
        self.emitted = []

    def fill(self, template_fn, ast):
        """
        Returns the template filled as filled by the ast values.
        """
        template_path = os.sep.join([
            self.template_root,
            template_fn
        ])
        code = Template(filename=template_path).render(
            namespace=ast["namespace"],
            nodes=ast["nodes"]
        )
        return code

    def to_file(self, code, output_path):
        """
        Write code to file.

        @Return Path to the file written.
        """
        fn = os.sep.join([self.output_root, output_path])
        with open(fn, 'w') as fd:
            fd.write(code)
        return fn

    def emit(self, template_fn, output_path, ast):
        """
        Generate code and store it to file.

        @Return Path to where the generated file..
        """
        code = self.fill(template_fn, ast)
        path = self.to_file(code, output_path)

        self.emitted.append(path)

        return path

def main(args):
    
    yaml_path = os.path.realpath(os.path.expandvars(os.path.expanduser(args.yaml)))

    ast = yaml.load(open(yaml_path))    # Load yaml
    fill_ast(ast)                       # Fill it out with default values

    output_root = os.path.realpath(os.path.expandvars(os.path.expanduser(os.sep.join([
        args.output_root,
        ast["namespace"]["name"],
        "ast"]
    ))))
    template_root = os.path.realpath(os.path.expandvars(os.path.expanduser(args.template_root)))

    for path in [output_root, template_root]:        # Check paths
        if not os.path.exists(path):
            print("Path(%s) does not exist." % path)
            return

    """
    print (output_root, template_root)

    generated = []
    generated += gen_nodes(ast, output_root, template_root)     # Nodes
    generated += gen_nodes_hh(ast, output_root, template_root)  # Nodes header
    generated += gen_visitor_visit_auto_hh_inc(                 # Visitor::visit(...)
        ast,
        output_root,
        template_root
    )
    generated += gen_evaluator_visit_auto_hh_inc(               # Evaluator::visit(...)
        ast,
        output_root,
        template_root
    )
    generated += gen_evaluator_visit_auto_cc(               # Evaluator::visit(...)
        ast,
        output_root,
        template_root
    )
    """
    emitter = Emitter(output_root, template_root)
    emitter.emit("nodes_hh.tpl", "nodes.hh", ast)
    emitter.emit("nodes_cc.tpl", "nodes.cc", ast)
    emitter.emit("visitor_visit_auto_hh_inc.tpl", "visitor/visitor_visit_auto_hh.inc", ast)
    emitter.emit("evaluator_visit_auto_hh_inc.tpl", "visitor/evaluator_visit_auto_hh.inc", ast)
    emitter.emit("evaluator_visit_auto_cc.tpl", "visitor/evaluator_visit_auto.cc", ast)

    pprint.pprint(emitter.emitted)

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
