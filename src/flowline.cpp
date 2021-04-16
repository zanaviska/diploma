#include <flowline.h>

#include <iostream>
#include <memory>

#include <input.h>
#include <terminal.h>
#include <proccess.h>

flowline::flowline(const std::vector<line> &lines, size_t index, cv::Point start, cv::Point end):
    block(block_type::flowline)
{
    std::cout << "Added flowline\n";

    _start = std::move(start);
    _end = std::move(end);

    // if next element is flowline
    for (size_t i = 2; i < lines.size(); i++)
        if (i != index)
        {
            if (is_equal(_end, lines[i].first))
            {
                child = std::make_shared<flowline>(lines, i, lines[i].first, lines[i].second);
                return;
            }
            if (is_equal(_end, lines[i].second))
            {
                child = std::make_shared<flowline>(lines, i, lines[i].second, lines[i].first);
                return;
            }
        }
    // if next element is input block
    for (int i = 2; i < lines.size(); i++)
        if (on_line(lines[i], _end) && !is_equal(lines[i].first, _end) &&
            !is_equal(lines[i].second, _end))
        {
            int l = -1, r = -1;
            for (int j = 2; j < lines.size(); j++)
                if (i != j)
                {
                    if (is_equal(lines[i].first, lines[j].first) ||
                        is_equal(lines[i].first, lines[j].second) ||
                        is_equal(lines[i].second, lines[j].first) ||
                        is_equal(lines[i].second, lines[j].second))
                    {
                        l = j;
                        std::swap(l, r);
                    }
                }

            // if there is less than 2 neighbor lines
            if (l == -1)
            {
                std::cout << start << ' ' << _end << '\n';
                std::cout << lines[i].first << ' ' << lines[i].second << '\n';
                std::cout << l << ' ' << r << '\n';
                break;
            }

            line left = lines[l];
            line right = lines[r];
            // if paralelllogram
            if (is_equal(cv::Point(std::max(left.first.x, left.second.x) -
                                   std::min(left.first.x, left.second.x),
                                   std::max(left.first.y, left.second.y) -
                                   std::min(left.first.y, left.second.y)),
                         cv::Point(std::max(right.first.x, right.second.x) -
                                   std::min(right.first.x, right.second.x),
                                   std::max(right.first.y, left.second.y) -
                                   std::min(right.first.y, right.second.y))))
            {
                // if square
                if (is_equal(left.first, cv::Point(left.second.x, left.first.y)))
                {
                    // process
                    child = std::make_shared<proccess>(lines, i, l, r);
                    return;
                }
                else
                {
                    // input
                    class input temp(lines, i, l, r);
                    child = std::make_shared<input>(temp);
                    return;
                }
            }
        }

    // if next element is terminal
    for (size_t i = 2; i < lines.size(); i++)
    {
        if (on_line(lines[i], end))
        {
            child = std::make_shared<terminal>(lines, i, lines.size() - 1);
            return;
        }
    }
    return;
}