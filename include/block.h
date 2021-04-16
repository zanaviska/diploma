#ifndef PROJECT_BLOCK_H
#define PROJECT_BLOCK_H

#include <memory>

class block
{
protected:
    enum block_type
    {
        terminal,
        flowline,
        input,
        proccess
    };
    const block_type _type;

    explicit block(block_type type): _type{type} {}
};

#endif // PROJECT_BLOCK_H
