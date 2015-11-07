
class ${name} : public Node {
public:
    ${name}(void);
    ${name}(${ctype} val);

    std::string dot_label(void);
    std::string dot_shape(void);
    std::string dot_color(void);
};

