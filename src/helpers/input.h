#pragma once
#include "../setup/input_t.h"
#include <fstream>
#include <functional>
#include <string>
#include <string_view>

/// Input helpers

/// Get full input as a single line
inline sv get_sv(const input_t& input)
{
    return sv { input.s, static_cast<size_t>(input.len) };
}

inline void parse_input(const input_t& input, std::function<void(const std::string_view&)> line_cb)
{
    for (size_t size = 0, i = 0; i < input.len; i++)
    {
        if (input.s[i] != '\n')
        {
            ++size;
        }
        else
        {
#ifdef __linux__
            std::string_view line { &input.s[i - size], size };
#elif _WIN32
            std::string_view line { &input.s[i - size], size - 1 }; // account for "\r\n"
#endif
            line_cb(line);
            size = 0;
        }
    }
}

inline input_t load_input(const std::string& filename)
{
    input_t input;

    std::ifstream file(filename.c_str(), std::ios::binary | std::ios::ate);
    input.len = file.tellg();
    if (input.len == -1)
    {
        perror("filesize failed");
        exit(EXIT_FAILURE);
    }

    file.seekg(0, std::ios::beg);
    input.s = new char[input.len];
    if (input.s == nullptr)
    {
        perror("allocation failed");
        exit(EXIT_FAILURE);
    }

    if (!file.read(input.s, input.len))
    {
        perror(filename.c_str());
        exit(EXIT_FAILURE);
    }

    return input;
}

inline void free_input(input_t& input)
{
    delete[] input.s;
}
