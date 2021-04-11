#ifndef PROJECT_BLOCK_H
#define PROJECT_BLOCK_H

#include <memory>

class block
{
protected:
    enum block_type
    {
        terminal,
        flowline
    };
    block_type type;
    std::unique_ptr<block> parent;
};

#endif // PROJECT_BLOCK_H
