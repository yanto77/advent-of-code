#include "advent2020.h"
#include <charconv>

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
        assert(false);
    }

    return i;
}

char to_char(const std::string_view& sv)
{
    return *(sv.data());
}
