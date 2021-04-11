#include <algorithm>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <functions.h>
#include <terminal.h>

using namespace cv;
using namespace std;

void print(std::shared_ptr<block> now)
{
    if()
}

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
    if (lines.size() < 3) return -1;

    std::sort(lines.begin(), lines.end(), [](auto lhs, auto rhs) {
        if (std::min(lhs.first.y, lhs.second.y) < std::min(rhs.first.y, rhs.second.y)) return true;
        if (std::min(lhs.first.y, lhs.second.y) > std::min(rhs.first.y, rhs.second.y)) return false;
        if (std::max(lhs.first.y, lhs.second.y) < std::max(rhs.first.y, rhs.second.y)) return true;
        if (std::max(lhs.first.y, lhs.second.y) > std::max(rhs.first.y, rhs.second.y)) return false;
        return std::min(lhs.first.x, lhs.second.x) < std::min(lhs.first.x, lhs.second.x);
    });

    cv::Point tl = lines[0].first;
    if (tl.x > lines[0].second.x) tl = lines[0].second;
    if (std::abs(lines[1].first.y - lines[1].second.y) >
        std::abs(lines[2].first.y - lines[2].second.y))
        std::swap(lines[1], lines[2]);
    cv::Point br = lines[1].first;
    if (br.x < lines[1].second.x) br = lines[1].second;
    std::unique_ptr<block> start = std::unique_ptr<block>(new terminal(tl, br));

    return 0;
}