#pragma once

#include <list>

#include <opencv2/core.hpp>

#include <block.h>
#include <functions.h>

class flowline : public block
{
    cv::Point _start;
    cv::Point _end;
    std::shared_ptr<block> child{nullptr};
    flowline(cv::Point start, cv::Point end) :
        block(block_type::flowline), _start{std::move(start)}, _end{std::move(end)}
    {
    }
    inline static std::list<flowline *> pending;

    friend class terminal;

public:
    static std::vector<std::shared_ptr<flowline>> visited;
    flowline(const std::vector<line> &lines, size_t index, const cv::Point &start,
             const cv::Point &end, const cv::Mat &);

    static std::shared_ptr<flowline> make(const std::vector<line> &lines, size_t index,
                                          const cv::Point &start, const cv::Point &end,
                                          const cv::Mat &);
    static void translate(std::shared_ptr<flowline>, std::vector<std::string>&);
};