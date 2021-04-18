#include <decision.h>

#include <iostream>

decision::decision(const std::vector<line> lines) : block(block_type::decision)
{
    std::cout << "Added decision making\n";
}