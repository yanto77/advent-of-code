#include "advent2020.h"

#include <map>
#include <string>
#include <iostream>

namespace
{
    //void parse_line(const std::string_view& s) noexcept
    //{
    //    // muted plum bags contain 3 shiny brown bags, 4 shiny teal bags.
    //    // [MP] bags contain 3 [SB] bag(s|).
    //    // [LR] = > 1 [BW] bag, 2 [MY] bags.
    //}
}

void day07(const input_t& input)
{
    std::map<std::string_view, std::string_view> v;

    parse_input(input, [&](const std::string_view& line) {

        const auto& [key, value] = split_single(line, " bags contain ");

        // std::cout << "line: " << line << std::endl;
        std::cout
            << "    key: " << key << "\t\t"
            << std::hash<std::string_view>{}(key) << "\t\t"
            << djb_hash(key) << "\t\t"  // << this one produces 1 collision!!
            << fnv1a_hash(key) << "\t\t"
            << std::endl;
        //std::cout << "    value: " << value << std::endl;
        // const auto& values = split_multi(value, ',');
        // for (auto asd : values)
        // {
        //     std::cout << "        - " << asd << std::endl;
        // }

        v.emplace(key, value);
    });
}

void day07_test()
{
}
