${name}::${name}(Node* left, Node* right) : Node(left, right) {}
void ${name}::eval(void)
{
    left()->eval();
    right()->eval();

    nls_operator_{op}(this, left(), right());
}
string ${name}::dot_label(void) { return "${name}"; }

