#include <algorithm>
#include <filesystem>
#include <iostream>
#include <set>
#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <flowline.h>
#include <functions.h>
#include <terminal.h>

namespace fs = std::filesystem;

std::vector<std::shared_ptr<flowline>> flowline::visited = std::vector<std::shared_ptr<flowline>>();

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        std::cerr << "wrong usage. Usage:\n./project <path to image> <path to output>";
        return -1;
    }

    std::string image_path = fs::absolute(argv[1]).string();

    // Loads an image
    cv::Mat src = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

    // Check if image is loaded fine
    if (src.empty())
    {
        printf(" Error opening image\n");
        return -1;
    }

    cv::Mat clone;
    cv::cvtColor(src, clone, cv::COLOR_GRAY2BGR);

    // fill word with white color
    auto words = text_getter(clone);
    for (auto word : words)
    {
        cv::rectangle(clone, word.second, cv::Scalar(255, 255, 255), cv::FILLED);
        cv::rectangle(src, word.second, cv::Scalar(255, 255, 255), cv::FILLED);
        std::cout << word.second << '\n';
    }
    auto lines = get_lines(src, image_path);

    // clear duplicates
    for (size_t i = 0; i < lines.size(); i++)
        for (size_t j = 0; j < lines.size(); j++)
            if (i != j)
            {
                // if lines are equal
                if ((is_equal(lines[i].first, lines[j].first) &&
                     is_equal(lines[i].second, lines[j].second)) ||
                    (is_equal(lines[i].first, lines[j].second) &&
                     is_equal(lines[i].second, lines[j].first)))
                {
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                // if lines are the same but one is shorter
                else if ((is_equal(lines[i].first, lines[j].first) &&
                          on_line(lines[i], lines[j].second)) ||
                         (is_equal(lines[i].second, lines[j].first) &&
                          on_line(lines[i], lines[j].second)) ||
                         (is_equal(lines[i].first, lines[j].second) &&
                          on_line(lines[i], lines[j].first)) ||
                         (is_equal(lines[i].second, lines[j].second) &&
                          on_line(lines[i], lines[j].first)))
                {
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                // if one line in the middle of another
                else if (on_line(lines[i], lines[j].first) && on_line(lines[i], lines[j].second))
                {
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                // if lines should be concatinated
                else if (on_line({lines[i].first, lines[j].first}, lines[i].second) &&
                         on_line({lines[i].first, lines[j].first}, lines[j].second) &&
                         on_line(lines[i], lines[j].second))
                {
                    lines[i].second = lines[j].first;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                else if (on_line({lines[i].first, lines[j].second}, lines[i].second) &&
                         on_line({lines[i].first, lines[j].second}, lines[j].first) &&
                         on_line(lines[i], lines[j].first))
                {
                    lines[i].second = lines[j].second;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                else if (on_line({lines[i].second, lines[j].first}, lines[i].first) &&
                         on_line({lines[i].second, lines[j].first}, lines[j].second) &&
                         on_line(lines[i], lines[j].second))
                {
                    lines[i].first = lines[j].first;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                else if (on_line({lines[i].second, lines[j].second}, lines[i].first) &&
                         on_line({lines[i].second, lines[j].second}, lines[j].first) &&
                         on_line(lines[i], lines[j].first))
                {
                    lines[i].first = lines[j].second;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
            }

    // sort elements and remove duplicates
    std::sort(lines.begin(), lines.end(),
              [](auto lhs, auto rhs)
              {
                  if (std::min(lhs.first.y, lhs.second.y) < std::min(rhs.first.y, rhs.second.y))
                      return true;
                  if (std::min(lhs.first.y, lhs.second.y) > std::min(rhs.first.y, rhs.second.y))
                      return false;
                  if (std::max(lhs.first.y, lhs.second.y) < std::max(rhs.first.y, rhs.second.y))
                      return true;
                  if (std::max(lhs.first.y, lhs.second.y) > std::max(rhs.first.y, rhs.second.y))
                      return false;
                  return std::min(lhs.first.x, lhs.second.x) < std::min(lhs.first.x, lhs.second.x);
              });

    // place horizontal line of first terminal on first two lines
    for (int i = 0; i < lines.size(); i++)
        if (is_equal(lines[i].first, {lines[i].first.x, lines[i].second.y}))
        {
            std::swap(lines[i], lines[0]);

            for (int j = i + 1; j < lines.size(); j++)
                if (is_equal(lines[j].first, {lines[j].first.x, lines[j].second.y}))
                {
                    std::swap(lines[j], lines[1]);
                    break;
                }
            break;
        }

    for (size_t i = 0; i < lines.size(); i++)
        cv::line(src, lines[i].first, lines[i].second, cv::Scalar(255, 255, 255), 7);

    flowline::visited = std::vector<std::shared_ptr<flowline>>(lines.size(), nullptr);

    auto start = std::make_shared<terminal>(lines, 0, 1, clone);

    std::vector<std::string> translate;
    terminal::translate(start, translate);

    std::ofstream fout(argv[2]);
    for(auto &i: short_form(translate))
        fout << i << '\n';

    return 0;
}