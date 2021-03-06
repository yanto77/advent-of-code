#include "advent2020.h"


struct input_line_t
{
    int min;
    int max;
    char ch;
    std::string_view password;
};

namespace
{
    bool operator==(const input_line_t& a, const input_line_t& b)
    {
        return a.min == b.min &&
            a.max == b.max &&
            a.ch == b.ch &&
            a.password == b.password;
    }

    input_line_t parse_line(const std::string_view& s) noexcept
    {
        static constexpr auto pattern = ctll::fixed_string { "([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)$" };
        auto m = ctre::match<pattern>(s);

        // NB, No error checking done.
        return { to_int<int>(m.get<1>().to_view()),
                 to_int<int>(m.get<2>().to_view()),
                 *(m.get<3>().to_view().data()),
                 m.get<4>().to_view() };
    }

    size_t count_occurences(const std::string_view& input, const char counted_ch)
    {
        return std::count_if(input.begin(), input.end(), [&](const char ch) { return ch == counted_ch; });
    }
}

output_t day02(const input_t& input)
{
    std::vector<input_line_t> data;
    data.reserve(1000);

    parse_input(input, [&](const std::string_view& line)
    {
        data.emplace_back(parse_line(line));
    });

    size_t i = 0; // Part 1
    size_t j = 0; // Part 2
    for (auto d : data)
    {
        // Part 1
        size_t asd = count_occurences(d.password, d.ch);
        if (d.min <= asd && asd <= d.max)
        {
            i++;
        }

        // Part 2
        bool pos1 = d.password[d.min - 1] == d.ch;
        bool pos2 = d.password[d.max - 1] == d.ch;
        if (pos1 ^ pos2)
        {
            j++;
        }
    }

    return { i, j };
}

void day02_test()
{
    assert(parse_line("2-3 f: pfff") == input_line_t({ 2, 3, 'f', "pfff" }));
    assert(parse_line("7-11 z: zzzzzzzzzzzz") == input_line_t({ 7, 11, 'z', "zzzzzzzzzzzz" }));
    assert(parse_line("6-19 g: gggggggggggggggggggg") == input_line_t({ 6, 19, 'g', "gggggggggggggggggggg" }));

    assert(count_occurences("abcdefg", 'c') == 1);
    assert(count_occurences("abababa", 'a') == 4);
    assert(count_occurences("abababa", 'b') == 3);
}
