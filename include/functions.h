#pragma once

#include <tuple>
#include <vector>

#include <opencv2/core.hpp>

std::vector<std::pair<cv::Point, cv::Point>> get_lines(cv::Mat image);