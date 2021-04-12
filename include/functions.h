#pragma once

#include <tuple>
#include <vector>

#include <opencv2/core.hpp>

#include <block.h>
#include <terminal.h>
#include <flowline.h>

using line = std::pair<cv::Point, cv::Point>;

std::vector<line> get_lines(cv::Mat image);
bool is_equal(cv::Point lhs, cv::Point rhs);

std::shared_ptr<terminal> make_terminal(const std::vector<line> &lines, size_t upper, size_t lower);
std::shared_ptr<flowline> make_flowline(const std::vector<line> &lines, size_t index, cv::Point start, cv::Point end);

std::vector<std::pair<std::string, cv::Rect>> text_getter(cv::Mat image);