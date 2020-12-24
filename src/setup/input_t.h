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
