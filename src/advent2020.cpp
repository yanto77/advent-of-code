#include "advent2020.h"
#include <charconv>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

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
    static void* backsplash = nullptr;

    // Allows solutions to read past the end of the input safely
    static constexpr size_t BACKSPLASH_SIZE = 1 << 20;

    int flags = MAP_PRIVATE | MAP_ANONYMOUS | (backsplash ? MAP_FIXED : 0);
    backsplash = mmap(backsplash, BACKSPLASH_SIZE, PROT_READ, flags, -1, 0);
    if (backsplash == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1)
    {
        perror(filename.c_str());
        exit(EXIT_FAILURE);
    }

    input.len = lseek(fd, 0, SEEK_END);
    if (input.len > BACKSPLASH_SIZE)
    {
        fprintf(stderr, "Why is your input so big?\n");
        exit(EXIT_FAILURE);
    }

    input.s = reinterpret_cast<char*>(mmap(backsplash, input.len, PROT_READ, MAP_PRIVATE | MAP_FIXED, fd, 0));
    if (input.s == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (input.s != backsplash)
    {
        fprintf(stderr, "Warning: Input not mapped at the expected location.\n");
    }

    if (close(fd) == -1)
    {
        perror(filename.c_str());
        exit(EXIT_FAILURE);
    }
}

void free_input(input_t& input)
{
    if (munmap(input.s, input.len) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
}
