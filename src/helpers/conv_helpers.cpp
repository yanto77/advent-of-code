#include "conv_helpers.h"
#include <charconv>
#include <stdexcept>

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
