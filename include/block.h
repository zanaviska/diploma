#ifndef PROJECT_BLOCK_H
#define PROJECT_BLOCK_H

#include <memory>
#include <vector>

class block
{
protected:
    enum block_type
    {
        terminal,
        flowline,
        input,
        proccess,
        decision
    };

    explicit block(block_type type) : _type{type} {}
    const block_type _type;
public:
    static void translate(std::shared_ptr<block> node, std::vector<std::string> &str);
};

#endif // PROJECT_BLOCK_H
