#pragma once

#include <opencv2/core.hpp>

#include <block.h>

class flowline : public block
{
    const cv::Point _start;
    const cv::Point _end;
public:
    std::shared_ptr<block> child{nullptr};
    flowline(cv::Point start, cv::Point end):
        block(block_type::flowline),
        _start{std::move(start)},
        _end{std::move(end)}
    {}
};