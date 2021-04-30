#include <input.h>

#include <iostream>

input::input(const std::vector<line> &lines, int upper, int left, int right, const cv::Mat & image) :
    block(block_type::input)
{
//    std::cout << "Added input\n";

    line l = lines[left];
    const line &u = lines[upper];
    // sort element in  l to know exact order of elements in it
    if (l.first.y > l.second.y) std::swap(l.first, l.second);

    int dx = l.second.x - l.first.x;
    int dy = l.second.y - l.first.y;
    // get bottom line of input block
    line bottom = {cv::Point(u.first.x + dx, u.first.y + dy),
                   cv::Point(u.second.x + dx, u.second.y + dy)};

    // find line which should be corresponding line
    for (size_t i = 0; i < lines.size(); i++)
        if (i != left && i != right)
        {
            // if one of line edge is on bottom line, but not in its edge
            if (on_line(bottom, lines[i].first) && !is_equal(lines[i].first, bottom.first) &&
                !is_equal(lines[i].first, bottom.second))
            {
                child = flowline::make(lines, i, lines[i].first, lines[i].second, image);
                return;
            }

            if (on_line(bottom, lines[i].second) && !is_equal(lines[i].second, bottom.first) &&
                !is_equal(lines[i].second, bottom.second))
            {
                child = flowline::make(lines, i, lines[i].second, lines[i].first, image);
                return;
            }
        }
}