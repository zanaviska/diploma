#ifndef PROJECT_TERMINAL_H
#define PROJECT_TERMINAL_H

#include <string>

#include <opencv2/core.hpp>

#include <block.h>
#include <functions.h>
#include <flowline.h>

class terminal : public block
{
    cv::Point _tl;
    cv::Point _br;

public:
    std::shared_ptr<::flowline> child;
    terminal(cv::Point tl, cv::Point br);
    terminal(const std::vector<line> &lines, size_t upper, size_t lower, const cv::Mat &);
    static void translate(std::shared_ptr<terminal>, std::vector<std::string>&);
};

#endif // PROJECT_TERMINAL_H
