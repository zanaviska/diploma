#include <block.h>

#include <iostream>

#include <flowline.h>
#include <terminal.h>
#include <input.h>
#include <proccess.h>
#include <decision.h>

void block::translate(std::shared_ptr<block> node, std::vector<std::string> &str)
{
    switch(node->_type)
    {
    case flowline:
        flowline::translate(static_pointer_cast<::flowline>(node), str);
        break;
    case terminal:
        terminal::translate(static_pointer_cast<::terminal>(node), str);
        break;
    case input:
        input::translate(static_pointer_cast<::input>(node), str);
        break;
    case proccess:
        proccess::translate(static_pointer_cast<::proccess>(node), str);
        break;
    case decision:
        decision::translate(static_pointer_cast<::decision>(node), str);
        break;
    default:
        std::cout << "some third\n";
    }
}