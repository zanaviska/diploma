#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#include <block.h>

class input : public block
{
public:
    std::shared_ptr<block> child{nullptr};
    input() : block(block_type::input){};
};

#endif // PROJECT_INPUT_H
