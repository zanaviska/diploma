#include "input.h"

#include <iostream>

input::input(const std::vector<line> &lines, int upper, int left, int right) :
    block(block_type::input)
{
    std::cout << "input\n";
}