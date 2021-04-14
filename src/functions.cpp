#include <algorithm>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

#include <functions.h>
#include <opencv2/highgui.hpp>

std::vector<std::pair<std::string, cv::Rect>> text_getter(cv::Mat image)
{
    using namespace cv;
    using namespace cv::dnn;
    int width = 320;
    int height = 320;
    TextDetectionModel_EAST detector("../resources/frozen_east_text_detection.pb");
    detector.setConfidenceThreshold(0.5).setNMSThreshold(0.4);

    // Parameters for Detection
    double detScale = 1.0;
    Size detInputSize = Size(width, height);
    Scalar detMean = Scalar(123.68, 116.78, 103.94);
    bool swapRB = true;
    detector.setInputParams(detScale, detInputSize, detMean, swapRB);

    // Detection
    std::vector<std::vector<Point>> detResults;
    detector.detect(image, detResults);

    std::vector<std::pair<std::string, cv::Rect>> res;
    res.reserve(detResults.size());

    for (auto &i : detResults)
    {
        res.push_back({"", boundingRect(i)});
    }
    return res;
}

// offset between points, to be equal
constexpr size_t threshold = 10;

bool is_equal(cv::Point lhs, cv::Point rhs)
{
    return std::abs(lhs.x - rhs.x) < threshold && std::abs(lhs.y - rhs.y) < threshold;
}

bool on_line(line l, cv::Point p)
{
    double a = std::hypot(l.first.x - l.second.x, l.first.y - l.second.y);
    double b = std::hypot(l.first.x - p.x, l.first.y - p.y);
    double c = std::hypot(l.second.x - p.x, l.second.y - p.y);
    double per = (a + b + c) / 2;
    double h;
    if (b * b > a * a + c * c || c * c > a * a + b * b)
        h = std::min(b, c);
    else
        h = 2 * sqrt(per * (per - a) * (per - b) * (per - c)) / a;
    // std::cout << h << '\n';
    return h < threshold;
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
    HoughLinesP(dst, linesP, 1, CV_PI / 180, 80, 30, 10); // runs the actual detection

    // store in res vector
    std::vector<line> res;
    res.reserve(linesP.size());
    std::transform(linesP.begin(), linesP.end(), std::back_inserter(res),
                   [](cv::Vec4i l) {
                       return std::pair{cv::Point(l[0], l[1]), cv::Point(l[2], l[3])};
                   });
    return res;
}

std::shared_ptr<terminal> make_terminal(const std::vector<line> &lines, size_t upper, size_t lower)
{
    std::cout << "\nAdded terminal1\n";
    const line &up_line = lines[upper];
    const line &lo_line = lines[lower];

    // create terminal
    auto res = std::make_shared<terminal>(
        up_line.first.x < up_line.second.x ? up_line.first : up_line.second,
        lo_line.second.x < lo_line.first.x ? lo_line.first : lo_line.second);

    //    std::cout << "our line:\n";
    //    std::cout << lo_line.first << ' ' << lo_line.second << "\n\n";
    // if our terminal isn't last that add child(flowline)
    if (lines.size() != lower + 1)
        for (size_t i = 2; i < lines.size(); i++)
        {
            //            std::cout << lines[i].first << ' ' << lines[i].second << '\n';
            if (on_line(lo_line, lines[i].first))
            {
                res->child = make_flowline(lines, i, lines[i].first, lines[i].second);
                break;
            }
            if (on_line(lo_line, lines[i].second))
            {
                res->child = make_flowline(lines, i, lines[i].second, lines[i].first);
                break;
            }
        }
    return res;
}

std::shared_ptr<input> make_input(const std::vector<line> &lines, int upper, int left, int right)
{
    std::cout << "input\n";
    return nullptr;
}

std::shared_ptr<flowline> make_flowline(const std::vector<line> &lines, size_t index,
                                        cv::Point start, cv::Point end)
{
    std::cout << "Added flowline\n";

    // create flowline
    auto res = std::make_shared<flowline>(std::move(start), end);

    // if next element is flowline
    for (size_t i = 2; i < lines.size(); i++)
        if (i != index)
        {
            if (is_equal(end, lines[i].first))
            {
                res->child = make_flowline(lines, i, lines[i].first, lines[i].second);
                return res;
            }
            if (is_equal(end, lines[i].second))
            {
                res->child = make_flowline(lines, i, lines[i].second, lines[i].first);
                return res;
            }
        }
    // if next element is input block
    for (int i = 2; i < lines.size(); i++)
        if (on_line(lines[i], end))
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
            line left = lines[l];
            line right = lines[r];
            //if paralelllogram
            if (is_equal(cv::Point(std::max(left.first.x, left.second.x) -
                                       std::min(left.first.x, left.second.x),
                                   std::max(left.first.y, left.second.y) -
                                       std::min(left.first.y, left.second.y)),
                         cv::Point(std::max(right.first.x, right.second.x) -
                                       std::min(right.first.x, right.second.x),
                                   std::max(right.first.y, left.second.y) -
                                       std::min(right.first.y, right.second.y))))
            {
                //if square
                if(is_equal(left.first, cv::Point(left.second.x, left.first.y)))
                {
                    // process
                    std::cout << "proccess\n";
                    return res;
                } else
                {
                    // input
                    res->child = make_input(lines, i, l, r);
                    return res;
                }

            }
        }

    // if next element is terminal
    for (size_t i = 2; i < lines.size(); i++)
    {
        if (on_line(lines[i], end))
        {
            res->child = make_terminal(lines, i, lines.size() - 1);
            return res;
        }
    }
    return res;
}