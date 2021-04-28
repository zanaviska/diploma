#pragma once

#include <memory>

#include <block.h>
#include <flowline.h>

class proccess : public block
{
    std::string _text;

public:
    std::shared_ptr<::flowline> child{nullptr};
    proccess(const std::vector<line> &lines, int upper, int left, int right, const cv::Mat &);
};
