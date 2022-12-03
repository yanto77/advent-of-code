#pragma once
#include <functional>
#include <string_view>

using str_view = std::string_view;

inline void parse_input(str_view input, std::function<void(str_view)> line_cb)
{
    for (size_t size = 0, i = 0; i < input.size(); i++)
    {
        if (input[i] != '\n')
        {
            ++size;
        }
        else
        {
#ifdef __linux__
            str_view line { &input[i - size], size };
#elif _WIN32
            str_view line { &input.s[i - size], size - 1 }; // account for "\r\n"
#endif
            line_cb(line);
            size = 0;
        }
    }
}
