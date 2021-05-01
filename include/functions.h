#pragma once

#include <tuple>
#include <vector>

#include <opencv2/core.hpp>

#include <block.h>

using line = std::pair<cv::Point, cv::Point>;

std::vector<line> get_lines(cv::Mat image, std::string str = "");
bool is_equal(cv::Point lhs, cv::Point rhs);
bool on_line(line l, cv::Point p);

std::vector<std::pair<std::string, cv::Rect>> text_getter(cv::Mat image);
std::string text_recognitor(const cv::Mat &);

void short_form(std::vector<std::string>& text);