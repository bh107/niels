#!/usr/bin/env python
import itertools
import argparse
import pprint
import glob
import yaml
import os

from mako.template import Template
from niels import camelize

def gen_node(yaml):
    namespace_name = yaml["namespace"]
    namespace_guard = namespace_name.upper()

    for node in yaml["nodes"]:
        node_name = node["name"]

        node_class = camelize(node_name) # Construct various valuas for the template
        node_shape = "box"
        node_guard = node_name.upper()

        hdr = Template(filename=os.sep.join(["templates", "node.hh.tpl"])).render(
            namespace_name=namespace_name,
            namespace_guard=namespace_guard,
            node_name=node_name,
            node_class=node_class,
            node_shape=node_shape,
            node_guard=node_guard
        )
        src = Template(filename=os.sep.join(["templates", "node.cc.tpl"])).render(
            namespace_name=namespace_name,
            namespace_guard=namespace_guard,

            node_name=node_name,
            node_class=node_class,
            node_shape=node_shape,
            node_guard=node_guard           
        )
        yield (node, hdr, src)

def main():
    parser = argparse.ArgumentParser(description='Autogenerate code based on ast.yaml.')
    parser.add_argument(
        "--yaml",
        type=str,
        help="Path to ast.yaml",
        default=os.sep.join([os.path.dirname(os.path.realpath(__file__)), "ast.yaml"])
    )

    args = parser.parse_args()              # Parse command-line arguments
    ast_yaml = yaml.load(open(args.yaml))   # Load yaml

    for node, hdr, src in gen_node(ast_yaml):
        src_path = os.sep.join(["out", "nls", "ast"])+os.sep+"%s.cc" % node["name"]
        hdr_path = os.sep.join(["out", "nls", "ast"])+os.sep+"%s.hh" % node["name"]
        with open(src_path, "w") as fd:
            fd.write(src)
        with open(hdr_path, "w") as fd:
            fd.write(hdr)


if __name__ == "__main__":
    main()
