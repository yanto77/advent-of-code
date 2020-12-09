#include "advent2020.h"
#include <3rd-party/ctre.hpp>
#include <charconv>
#include <stdexcept>
#include <iostream>

struct input_line_t
{
    int min;
    int max;
    char ch;
    std::string_view password;
};

namespace
{
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

    input_line_t parse_line(const std::string_view& s) noexcept
    {
        // Input format:
        //   %i-%i %c: %s\n
        // Examples:
        //   2-3 f: pfff
        //   7-11 z: zzzzzzzzzzzz
        //   6-19 g: gggggggggggggggggggg

        static constexpr auto pattern = ctll::fixed_string{ "([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)$" };
        auto m = ctre::match<pattern>(s);

        // NB, No error checking done.
        return
        {
            to_int(m.get<1>().to_view()),
            to_int(m.get<2>().to_view()),
            to_char(m.get<3>().to_view()),
            m.get<4>().to_view()
        };
    }

    std::vector<input_line_t> parse_input(const input_t& input)
    {
        std::vector<input_line_t> v;
        v.reserve(1000);

        for (size_t size = 0, i = 0; i < input.len; i++)
        {
            if (input.s[i] != '\n')
            {
                ++size;
            }
            else
            {
                std::string_view line { &input.s[i-size], size };
                v.emplace_back(parse_line(line));

                size = 0;
            }
        }

        return v;
    }
}

void day02(input_t input)
{
    printf("Day 02: todo\n\n");

    const auto& data = parse_input(input);

    for (auto d: data)
    {
        printf("min: %d, max: %d, char: %c, password: ", d.min, d.max, d.ch);
        std::cout << d.password << std::endl;
    }
}
