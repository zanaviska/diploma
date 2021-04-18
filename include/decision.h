#pragma once

#include <block.h>
#include <flowline.h>

class decision : public block
{
public:
    std::shared_ptr<::flowline> truth;
    std::shared_ptr<::flowline> lie;
    decision(const std::vector<line> lines);
};