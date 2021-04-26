#include <decision.h>

#include <iostream>

decision::decision(const std::vector<line> &lines, cv::Point up, int lu, int ru,
                   const cv::Mat &image) :
    block(block_type::decision)
{
    std::cout << "Added decision making\n";
    // swap left and right side so it is really left and right
    if (std::max(lines[ru].first.x, lines[ru].second.x) <
        std::max(lines[lu].first.x, lines[lu].second.x))
        std::swap(lu, ru);

    // left point of rhomb
    auto left_point = lines[lu].first;
    if (is_equal(left_point, up)) left_point = lines[lu].second;

    // right point of rhomb
    auto right_point = lines[ru].first;
    if (is_equal(right_point, up)) right_point = lines[ru].second;

    // neighbor from left and right
    int l1 = -1, l2 = -1, r1 = -1, r2 = -1;
    for (size_t i = 0; i < lines.size(); i++)
        if (i != lu && i != ru)
        {
            // if current line goes to left
            if (is_equal(left_point, lines[i].first) || is_equal(left_point, lines[i].second))
            {
                l1 = i;
                std::swap(l1, l2);
            }

            // if current line goes to the right
            if (is_equal(right_point, lines[i].first) || is_equal(right_point, lines[i].second))
            {
                r1 = i;
                std::swap(r1, r2);
            }
        }

    // if there is line goes to bottom
    if (r1 == -1)
    {
        // bottom point of rhomb
        auto bottom_point = lines[r2].first;
        if (is_equal(bottom_point, right_point)) bottom_point = lines[r2].second;

        for (size_t i = 0; i < lines.size(); i++)
            if (i != l1 && i != l2 && i != r2 &&
                (is_equal(bottom_point, lines[i].first) || is_equal(bottom_point, lines[i].second)))
            {
                r1 = i;
                break;
            }
        right_point = bottom_point;
    }
    else
    // r2 should hold rhomb side, not flowline line
    {
        if (std::max(lines[r1].first.y, lines[r1].second.y) >
            std::max(lines[r2].first.y, lines[r2].second.y))
            std::swap(r1, r2);
    }

    // place rhomb side in l2, l1 keep flowline
    if (std::max(lines[l1].first.y, lines[l1].second.y) >
        std::max(lines[l2].first.y, lines[l2].second.y))
        std::swap(l1, l2);

    // add 2 separate childs
    // add truth
    if (is_equal(right_point, lines[r1].first))
        truth = flowline::make(lines, r1, right_point, lines[r1].second, image);
    else
        truth = flowline::make(lines, r1, right_point, lines[r1].first, image);

    // add lie
    if (is_equal(left_point, lines[l1].first))
        truth = flowline::make(lines, l1, left_point, lines[l1].second, image);
    else
        truth = flowline::make(lines, l1, left_point, lines[l1].first, image);
}