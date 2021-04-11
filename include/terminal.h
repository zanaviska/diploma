#ifndef PROJECT_TERMINAL_H
#define PROJECT_TERMINAL_H

#include <string>

#include <opencv2/core.hpp>

#include <block.h>

class terminal: public block
{
    const cv::Point _tl;
    const cv::Point _br;
    const std::string _text;

public:
    std::shared_ptr<block> child;
    terminal(cv::Point tl, cv::Point br);
};

#endif // PROJECT_TERMINAL_H
