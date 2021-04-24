#pragma once

#include <block.h>
#include <flowline.h>

class decision : public block
{
public:
    std::shared_ptr<::flowline> truth;
    std::shared_ptr<::flowline> lie;
    // l mean left, r mean right, u mean up
    decision(const std::vector<line> &lines, cv::Point up, int lu, int ru);
};