#include "advent2020.h"
#include <charconv>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <fstream>

namespace
{
    /// Convert single char to hex
    // Source: https://stackoverflow.com/a/3382894
    int hex_to_dec(unsigned char hex_digit)
    {
        static const signed char hex_values[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
            -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        };

        int value = hex_values[hex_digit];
        if (value == -1)
            throw std::invalid_argument("invalid hex digit");

        return value;
    }
}

void parse_input(const input_t& input, std::function<void(const std::string_view&)> line_cb)
{
    for (size_t size = 0, i = 0; i < input.len; i++)
    {
        if (input.s[i] != '\n')
        {
            ++size;
        }
        else
        {
            std::string_view line { &input.s[i-size], size };
            line_cb(line);
            size = 0;
        }
    }
}

int to_int(const std::string_view& sv)
{
    int i;
    auto result = std::from_chars(sv.data(), sv.data() + sv.size(), i);
    if (result.ec == std::errc::invalid_argument)
    {
        return -1;
    }

    return i;
}

char to_char(const std::string_view& sv)
{
    return *(sv.data());
}

int hex_to_dec(const std::string_view& sv)
{
    return hex_to_dec(sv[1]) * 1048576 // 16^5
         + hex_to_dec(sv[2]) * 65536   // 16^4
         + hex_to_dec(sv[3]) * 4096    // 16^3
         + hex_to_dec(sv[4]) * 256     // 16^2
         + hex_to_dec(sv[5]) * 16      // 16^1
         + hex_to_dec(sv[6]) * 1;      // 16^0
}

void load_input(input_t& input, const std::string& filename)
{
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
}

void free_input(input_t& input)
{
    delete[] input.s;
}

void print_bits(uint8_t trg)
{
    for (int i = 0; i < 8; ++i)
    {
        printf(get_bit(trg, i) ? "1" : "0");
    }
}
