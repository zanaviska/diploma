#include <algorithm>
#include <functions.h>

#include <opencv2/imgproc.hpp>

// offset between points, to be equal
constexpr size_t threshold = 10;

bool is_equal(cv::Point lhs, cv::Point rhs)
{
    return std::abs(lhs.x - rhs.x) < threshold && std::abs(lhs.y - rhs.y) < threshold;
}

bool on_line(line l, cv::Point p)
{
    return std::abs(l.first.y - p.y) < threshold && std::abs(l.second.y - p.y) < threshold &&
           std::min(l.first.x, l.second.x) < p.x && p.x < std::max(l.second.x, l.first.x);
}

std::vector<line> get_lines(cv::Mat src)
{
    // Check if image is loaded fine
    if (src.empty()) return {};

    cv::Mat dst;
    // Edge detection
    cv::Canny(src, dst, 50, 200, 3);

    // Probabilistic Line Transform
    std::vector<cv::Vec4i> linesP;                        // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection

    // store in res vector
    std::vector<line> res;
    res.reserve(linesP.size());
    std::transform(linesP.begin(), linesP.end(), std::back_inserter(res), [](cv::Vec4i l) {
        return std::pair{cv::Point(l[0], l[1]), cv::Point(l[2], l[3])};
    });
    return res;
}

std::shared_ptr<terminal> make_terminal(const std::vector<line> &lines, size_t upper, size_t lower)
{
    const line &up_line = lines[upper];
    const line &lo_line = lines[lower];

    // create terminal
    auto res = std::make_shared<terminal>(
        up_line.first.x < up_line.second.x ? up_line.first : up_line.second,
        lo_line.second.x < lo_line.first.x ? lo_line.first : lo_line.second);

    // if our terminal isn't last that add child
    if (lines.size() != lower - 1)
        for(size_t i = 1; i < lines.size(); i++)
        {
            if (on_line(lo_line, lines[i].first)) {
                res->child = make_flowline(lines,i, lines[i].first, lines[i].second);
                break;
            }
            if (on_line(lo_line, lines[i].second)) {
                res->child = make_flowline(lines, i, lines[i].second, lines[i].second);
                break;
            }
        }
    return res;
}

std::shared_ptr<flowline> make_flowline(const std::vector<line> &lines, size_t index,
                                        cv::Point start, cv::Point end)
{
    //create flowline
    auto res = std::make_shared<flowline>(start, end);

    //if next element is flowline
    for(size_t i = 1; i < lines.size(); i++)
        if(i != index)
        {
            if(is_equal(end, lines[i].first))
            {
                res->child = make_flowline(lines, i, lines[i].first, lines[i].second);
                return res;
            }
            if(is_equal(end, lines[i].second))
            {
                res->child = make_flowline(lines, i, lines[i].second, lines[i].first);
                return res;
            }
        }
    for(size_t i = 1; i < lines.size(); i++)
        if(on_line(lines[i], end))
        {
            res->child = make_terminal(lines, i, lines.size() - 1);
            return res;
        }
    return res;
}