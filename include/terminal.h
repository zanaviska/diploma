#ifndef PROJECT_TERMINAL_H
#define PROJECT_TERMINAL_H

#include <string>

#include <opencv2/core.hpp>

#include "block.h"

class terminal
{
    cv::Point tl;
    cv::Point br;
    std::string text;
    block child;
};

#endif // PROJECT_TERMINAL_H
