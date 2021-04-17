#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#include <block.h>
#include <flowline.h>
#include <functions.h>

class input : public block
{
public:
    std::shared_ptr<block> child{nullptr};
    input(const std::vector<line> &lines, int upper, int left, int right);
};

#endif // PROJECT_INPUT_H
