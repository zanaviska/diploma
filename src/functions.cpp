#include <algorithm>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <functions.h>
#include <input.h>
#include <terminal.h>

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
    HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 30, 10); // runs the actual detection

    // store in res vector
    std::vector<line> res;
    res.reserve(linesP.size());
    std::transform(linesP.begin(), linesP.end(), std::back_inserter(res),
                   [](cv::Vec4i l) {
                       return std::pair{cv::Point(l[0], l[1]), cv::Point(l[2], l[3])};
                   });
    return res;
}
