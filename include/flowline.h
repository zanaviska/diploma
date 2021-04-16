#pragma once

#include <opencv2/core.hpp>

#include <block.h>
#include <functions.h>

class flowline : public block
{
    cv::Point _start;
    cv::Point _end;
public:
    std::shared_ptr<block> child{nullptr};
    flowline(cv::Point start, cv::Point end):
        block(block_type::flowline),
        _start{std::move(start)},
        _end{std::move(end)}
    {}
    flowline(const std::vector<line> &lines, size_t index, cv::Point start, cv::Point end);
};