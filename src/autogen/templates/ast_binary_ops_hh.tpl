class {op} : public Node {
public:
    {op}(Node* in1, Node* in2);
    void eval(void);
    std::string dot_label(void);
};

