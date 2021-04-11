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
    const block_type _type;

    block(block_type type): _type{type} {}
};

#endif // PROJECT_BLOCK_H
