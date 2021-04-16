#ifndef PROJECT_TERMINAL_H
#define PROJECT_TERMINAL_H

#include <string>

#include <opencv2/core.hpp>

#include <block.h>
#include <functions.h>

class terminal: public block
{
    cv::Point _tl;
    cv::Point _br;
    const std::string _text;

public:
    std::shared_ptr<block> child;
    terminal(cv::Point tl, cv::Point br);
    terminal(const std::vector<line> &lines, size_t upper, size_t lower);
};

#endif // PROJECT_TERMINAL_H
