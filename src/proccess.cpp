#include <proccess.h>

#include <iostream>

proccess::proccess(const std::vector<line> &lines, int upper, int left, int right, const cv::Mat & image) :
    block(block_type::proccess)
{
//    std::cout << "make proccess\n";
    int diff = std::abs(lines[left].first.y - lines[left].second.y);
    line bottom = {cv::Point(lines[upper].first.x, lines[upper].first.y + diff),
                   cv::Point(lines[upper].second.x, lines[upper].first.y + diff)};
    for (size_t i = 0; i < lines.size(); i++)
        if (i != left && i != right)
        {
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


void proccess::translate(std::shared_ptr<proccess> node, std::vector<std::string> &res)
{
    res.emplace_back("label_" + std::to_string((size_t)node.get()) + ":");

    res.emplace_back(node->_text);

    flowline::translate(node->child, res);
}