#include <algorithm>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <functions.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    // Loads an image
    cv::Mat src = imread("../flowchart1.jpg", IMREAD_GRAYSCALE);
    // Check if image is loaded fine
    if (src.empty()) {
        printf(" Error opening image\n");
        return -1;
    }

    auto lines = get_lines(src);

    std::sort(lines.begin(), lines.end(), [](auto lhs, auto rhs) {
        if (std::min(lhs.first.y, lhs.second.y) < std::min(rhs.first.y, rhs.second.y)) return true;
        if (std::min(lhs.first.y, lhs.second.y) > std::min(rhs.first.y, rhs.second.y)) return false;
        if (std::max(lhs.first.y, lhs.second.y) < std::max(rhs.first.y, rhs.second.y)) return true;
        if (std::max(lhs.first.y, lhs.second.y) > std::max(rhs.first.y, rhs.second.y)) return false;
        return std::min(lhs.first.x, lhs.second.x) < std::min(lhs.first.x, lhs.second.x);
    });

    cv::Mat copy;
    cv::cvtColor(src, copy, cv::COLOR_GRAY2BGR);
    for (auto &i : lines)
        cv::line(copy, i.first, i.second, cv::Scalar(255, 0, 0), 2);

    // Show results
    imshow("Source", src);
    // imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", copy);
    // Wait and Exit
    waitKey();
    return 0;
}