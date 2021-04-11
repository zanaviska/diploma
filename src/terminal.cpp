//
// Created by user on 22.05.2021.
//

#include "terminal.h"

terminal::terminal(cv::Point tl, cv::Point br) :
    block(block_type::terminal), _tl{std::move(tl)}, _br{std::move(br)}, _text{""}
{
}