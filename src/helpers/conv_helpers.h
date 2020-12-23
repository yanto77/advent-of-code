#pragma once
#include <string_view>
#include <cstdint>
#include <charconv>

/// Conversion helpers

template<typename T>
T to_int(const std::string_view& sv)
{
    T i;
    auto result = std::from_chars(sv.data(), sv.data() + sv.size(), i);
    if (result.ec == std::errc::invalid_argument)
    {
        return -1;
    }

    return i;
}

char to_char(const std::string_view& sv);

/// Convert full 6-char hex
/// Expects input in form of "#123abc"
int hex_to_dec(const std::string_view& sv);
