#include <functions.h>

#include <algorithm>

#include <opencv2/imgproc.hpp>

std::vector<std::pair<cv::Point, cv::Point>> get_lines(cv::Mat src)
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
    std::vector<std::pair<cv::Point, cv::Point>> res;
    res.reserve(linesP.size());
    std::transform(linesP.begin(), linesP.end(), std::back_inserter(res), [](cv::Vec4i l) {
        return std::pair{cv::Point(l[0], l[1]), cv::Point(l[2], l[3])};
    });
    return res;
}