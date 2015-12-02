/*
    NOTE: This file is generated by:

    * Declaration:  autogen/ast.yaml
    * Logic:        autogen/ast.py
    * Template:     autogen/templates/nodes_hh.tpl

    Modify the above files to persist changes.

    Locally changing this file is possible, however, be aware that cmake
    picks up changes to ANY file in autogen/ and thereby
    overwrites ALL local changes to ANY generated file.
*/
#ifndef ${namespace["guard"]}_AST_NODES_HH
#define ${namespace["guard"]}_AST_NODES_HH
#include <nls/ast/node.hh>

namespace ${namespace["name"]} {
namespace ast {
% for node in nodes:

class ${node["class"]} : public Node {
public:
    ${node["class"]}(void);

    ${node["class"]}(Node* left);

    ${node["class"]}(Node* left, Node* right);

% if node["constr_lval"]:
    ${node["class"]}(const char* lval);
% endif

    void accept(Visitor& visitor);

    std::string dot_label(void);

    std::string dot_shape(void);

};
% endfor

}}

#endif //${namespace["guard"]}_AST_${node["guard"]}_HH