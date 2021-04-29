#include <flowline.h>

#include <iostream>
#include <memory>

#include <decision.h>
#include <input.h>
#include <proccess.h>
#include <terminal.h>

flowline::flowline(const std::vector<line> &lines, size_t index, const cv::Point &start,
                   const cv::Point &end, const cv::Mat &image) :
    block(block_type::flowline)
{
    std::cout << "Added flowline\n";

    _start = std::move(start);
    _end = std::move(end);

    // potential neighbors
    int neight1 = -1;
    int neight2 = -1;

    // if next element is flowline
    for (size_t i = 2; i < lines.size(); i++)
        if (i != index)
        {
            if (is_equal(_end, lines[i].first))
            {
                neight1 = i;
                std::swap(neight1, neight2);
            }
            if (is_equal(_end, lines[i].second))
            {
                neight1 = i;
                std::swap(neight1, neight2);
            }
        }

    // if we founded 2 lines -- we found decision block
    if (neight1 != -1 && neight2 != -1)
    {
        child = std::make_shared<::decision>(lines, _end, neight1, neight2, image);
        return;
    }

    // if we found 1 line -- we found flowline
    if (neight2 != -1)
    {
        if (is_equal(_end, lines[neight2].first))
            child =
                flowline::make(lines, neight2, lines[neight2].first, lines[neight2].second, image);
        else
            child =
                flowline::make(lines, neight2, lines[neight2].second, lines[neight2].first, image);
        return;
    }

    // if next element is starting with single line
    for (int i = 2; i < lines.size(); i++)
        if (on_line(lines[i], _end) && !is_equal(lines[i].first, _end) &&
            !is_equal(lines[i].second, _end))
            // if next elem goes to block
            if (is_equal(lines[i].first, cv::Point(lines[i].first.x, lines[i].second.y)))
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
                if (l == -1) break;

                // if this is terminal operator
                if (l >= lines.size() - 3 || r >= lines.size() - 3) break;

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
                        child = std::make_shared<::proccess>(lines, i, l, r, image);
                        return;
                    }
                    else
                    {
                        // input
                        child = std::make_shared<::input>(lines, i, l, r, image);
                        return;
                    }
                }
            }
            else
            // if next elem goes into another flowline
            {
                std::cout << "insert in line\n";
                // if there is a visited line which we will make our child, than make it
                for (auto &new_child : visited)
                    if (new_child)
                    {
                        if ((is_equal(new_child->_start, lines[i].first) &&
                             is_equal(new_child->_end, lines[i].second)) ||
                            (is_equal(new_child->_start, lines[i].second) &&
                             is_equal(new_child->_end, lines[i].first)))
                        {
                            child = new_child;
                            return;
                        }
                    }

                // if there is no visited line, which should be our child, add our line to child
                // pending
                pending.push_back(this);
            }

    // if next element is terminal
    for (size_t i = 2; i < lines.size(); i++)
    {
        if (on_line(lines[i], _end) && !is_equal(lines[i].first, _end) &&
            !is_equal(lines[i].second, _end))
        {
            child = std::make_shared<::terminal>(lines, i, lines.size() - 1, image);
            return;
        }
    }
    return;
}
std::shared_ptr<flowline> flowline::make(const std::vector<line> &lines, size_t index,
                                         const cv::Point &start, const cv::Point &end,
                                         const cv::Mat &image)
{
    visited[index] = std::make_shared<flowline>(lines, index, start, end, image);
    return visited[index];
}
