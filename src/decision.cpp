#include <decision.h>

#include <iostream>

decision::decision(const std::vector<line> lines, cv::Point up, int lu, int ru) :
    block(block_type::decision)
{
    std::cout << "Added decision making\n";
}