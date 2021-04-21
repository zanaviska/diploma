#pragma once

#include <tuple>
#include <vector>

#include <opencv2/core.hpp>

//#include "proccess.h"
#include <block.h>
//#include <flowline.h>
//#include <input.h>
//#include <terminal.h>

using line = std::pair<cv::Point, cv::Point>;

std::vector<line> get_lines(cv::Mat image, std::string str = "");
bool is_equal(cv::Point lhs, cv::Point rhs);
bool on_line(line l, cv::Point p);

//std::shared_ptr<terminal> make_terminal(const std::vector<line> &lines, size_t upper, size_t lower);
//std::shared_ptr<flowline> make_flowline(const std::vector<line> &lines, size_t index, cv::Point start, cv::Point end);
//std::shared_ptr<input> make_input(const std::vector<line> &lines, int upper, int left, int right);
//std::shared_ptr<proccess> make_process(const std::vector<line> &lines, int upper, int left, int right);

std::vector<std::pair<std::string, cv::Rect>> text_getter(cv::Mat image);