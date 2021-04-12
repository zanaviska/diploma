#include <algorithm>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <functions.h>
#include <terminal.h>

int main(int argc, char **argv)
{
    // Loads an image
    cv::Mat src = cv::imread("../flowchart2.jpg", cv::IMREAD_GRAYSCALE);
    // Check if image is loaded fine
    if (src.empty()) {
        printf(" Error opening image\n");
        return -1;
    }


    cv::Mat clone;
    cv::cvtColor(src, clone, cv::COLOR_GRAY2BGR);

    auto words = text_getter(clone);
    for(auto word: words)
    {
        cv::rectangle(clone, word.second, cv::Scalar(0, 0, 255), cv::FILLED);
        cv::rectangle(src, word.second, cv::Scalar(255, 255, 255), cv::FILLED);
    }

    auto lines = get_lines(src);
    if (lines.size() < 3) return -1;

    //sort elements and remove duplicates
    std::sort(lines.begin(), lines.end(), [](auto lhs, auto rhs) {
        if (std::min(lhs.first.y, lhs.second.y) < std::min(rhs.first.y, rhs.second.y)) return true;
        if (std::min(lhs.first.y, lhs.second.y) > std::min(rhs.first.y, rhs.second.y)) return false;
        if (std::max(lhs.first.y, lhs.second.y) < std::max(rhs.first.y, rhs.second.y)) return true;
        if (std::max(lhs.first.y, lhs.second.y) > std::max(rhs.first.y, rhs.second.y)) return false;
        return std::min(lhs.first.x, lhs.second.x) < std::min(lhs.first.x, lhs.second.x);
    });
    auto it = std::unique(lines.begin(), lines.end(), [](line lhs, line rhs) {
        return (is_equal(lhs.first, rhs.first) && is_equal(lhs.second, rhs.second)) ||
               (is_equal(lhs.first, rhs.second) && is_equal(lhs.second, rhs.first));
    });
    lines.erase(it, lines.end());

    std::cout << "lines\n";
    for (auto &i : lines)
    std::cout << i.first << ' ' << i.second << '\n';

    for (size_t i = 0; i < lines.size(); i++)
        switch (i) {
        case 0:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(0, 255, 0), 2);
            break;

        default:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(255, 0, 0), 2);
        }

    if (std::abs(lines[1].first.y - lines[1].second.y) >
        std::abs(lines[2].first.y - lines[2].second.y))
        std::swap(lines[1], lines[2]);

    auto start = make_terminal(lines, 0, 1);

    cv::imshow("", clone);
    cv::imshow("1", src);
    cv::waitKey();
    return 0;
}