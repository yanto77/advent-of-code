#pragma once
#include <fstream>
#include <functional>
#include <string>
#include <string_view>
#include <filesystem>

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

// Ref: https://blog.insane.engineer/post/cpp_read_file_into_string/
inline std::string load_input(str_view filename)
{
    // Sanity check
    if (!std::filesystem::is_regular_file(filename))
        return { };

    // Open the file
    // Note that we have to use binary mode as we want to return a string
    // representing matching the bytes of the file on the file system.
    std::ifstream file(filename.data(), std::ios::in | std::ios::binary);
    if (!file.is_open())
        return { };

    // Read contents
    std::string content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

    // Close the file
    file.close();

    return content;
}
