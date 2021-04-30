#pragma once

#include <block.h>
#include <flowline.h>

class decision : public block
{
    std::string _text{"decision"};

public:
    std::shared_ptr<::flowline> truth;
    std::shared_ptr<::flowline> lie;
    // l mean left, r mean right, u mean up
    decision(const std::vector<line> &lines, cv::Point up, int lu, int ru, const cv::Mat &);

    static void translate(std::shared_ptr<decision>, std::vector<std::string>&);
};