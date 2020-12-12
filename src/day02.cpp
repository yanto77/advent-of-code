#include "advent2020.h"
#include <3rd-party/ctre.hpp> // https://github.com/hanickadot/compile-time-regular-expressions

struct input_line_t
{
    int min;
    int max;
    char ch;
    std::string_view password;
};

namespace
{
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

    size_t count_occurences(const std::string_view& input, const char& ch)
    {
        return std::count_if(input.begin(), input.end(),
            [&](const char& asd)
            {
                return asd == ch;
            });
    }
}

void day02(const input_t& input)
{
    std::vector<input_line_t> data;
    data.reserve(1000);

    parse_input(input, [&](const std::string_view& line)
    {
        data.emplace_back(parse_line(line));
    });

    int i = 0; // Part 1
    int j = 0; // Part 2
    for (auto d: data)
    {
        // Part 1
        size_t asd = count_occurences(d.password, d.ch);
        if (d.min <= asd && asd <= d.max)
        {
            i++;
        }

        // Part 2
        bool pos1 = d.password[d.min-1] == d.ch;
        bool pos2 = d.password[d.max-1] == d.ch;
        if (pos1 ^ pos2)
        {
            j++;
        }
    }

    printf("part 1: %d, part 2: %d", i, j);
    assert(i == 465);
    assert(j == 294);
}
