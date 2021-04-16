#include "input.h"

#include <iostream>

input::input(const input &x) : block(block_type::input)
{
    child = std::move(x.child);
}

input::input(const std::vector<line> &lines, int upper, int left, int right) :
    block(block_type::input)
{
    std::cout << "input\n";
}