#include "terminal.h"

#include <flowline.h>
#include <iostream>

terminal::terminal(const std::vector<line> &lines, size_t upper, size_t lower,
                   const cv::Mat &image) :
    block(block_type::terminal)
{
    std::cout << "\nAdded terminal1\n";
    const line &up_line = lines[upper];
    const line &lo_line = lines[lower];

    _tl = up_line.first.x < up_line.second.x ? up_line.first : up_line.second;
    _br = lo_line.second.x < lo_line.first.x ? lo_line.first : lo_line.second;

    // if our terminal isn't last that add child(flowline)
    if (lower + 3 <= lines.size())
        for (size_t i = 2; i < lines.size(); i++)
        {
            if (on_line(lo_line, lines[i].first) && !is_equal(lo_line.first, lines[i].first) &&
                !is_equal(lo_line.second, lines[i].first))
            {
                child = flowline::make(lines, i, lines[i].first, lines[i].second, image);
                break;
            }
            if (on_line(lo_line, lines[i].second) && !is_equal(lo_line.first, lines[i].second) &&
                !is_equal(lo_line.second, lines[i].second))
            {
                child = flowline::make(lines, i, lines[i].second, lines[i].first, image);
                break;
            }
        }

    // and on the end of terminal operator it's time to complete all pending flowline
    for (auto it = flowline::pending.begin(); it != flowline::pending.end();)
        for (auto &new_child : flowline::visited)
            if (new_child && on_line({new_child->_start, new_child->_end}, (*it)->_end))
            {
                (*it)->child = new_child;
                it = flowline::pending.erase(it);
            }
            else
                it++;
}

terminal::terminal(cv::Point tl, cv::Point br) :
    block(block_type::terminal), _tl{std::move(tl)}, _br{std::move(br)}, _text{""}
{
}