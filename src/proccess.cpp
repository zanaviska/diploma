#include <proccess.h>

#include <iostream>

proccess::proccess(const std::vector<line> &lines, int upper, int left, int right) :
    block(block_type::proccess)
{
    std::cout << "make proccess\n";
}