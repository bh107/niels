${name}::${name}(Node* left) : Node(left) {}
void ${name}::eval(void)
{
    left()->eval();

    nls_operator_{op}(this, left());
}
string ${name}::dot_label(void) { return "${name}"; }

