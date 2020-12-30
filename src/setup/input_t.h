#pragma once

#if defined(_MSC_VER)
    #include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
    #include <unistd.h>
#endif

struct input_t
{
    char* s;
    ssize_t len;
};

struct output_t
{
    size_t part1 = 0;
    size_t part2 = 0;
};
