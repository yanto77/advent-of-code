#include "advent2020.h"

#include <map>
#include <string>
#include <iostream>

namespace
{
    // D. J. Bernstein hash function
    // Modified from: https://codereview.stackexchange.com/a/85745
    size_t djb_hash(const std::string_view& sv)
    {
        size_t hash = 5381;
        for (auto ch: sv)
        {
            hash = 33 * hash ^ static_cast<unsigned char>(ch);
        }
        return hash;
    }

    // Fowler/Noll/Vo (FNV) hash function, variant 1a
    // Modified from: https://codereview.stackexchange.com/a/85745
    size_t fnv1a_hash(const std::string_view& sv)
    {
        size_t hash = 0x811c9dc5;
        for (auto ch: sv)
        {
            hash ^= static_cast<unsigned char>(ch);
            hash *= 0x01000193;
        }
        return hash;
    }


    //void parse_line(const std::string_view& s) noexcept
    //{
    //    // muted plum bags contain 3 shiny brown bags, 4 shiny teal bags.
    //    // [MP] bags contain 3 [SB] bag(s|).
    //    // [LR] = > 1 [BW] bag, 2 [MY] bags.
    //}

    std::pair<std::string_view, std::string_view> split_single(const std::string_view& input, const std::string_view& delim)
    {
        size_t lhs_end = input.find(delim);
        size_t rhs_start = lhs_end + delim.size();

        std::string_view lhs{ &input[0], lhs_end };
        std::string_view rhs{ &input[rhs_start], (input.size() - rhs_start) };

        return { lhs, rhs };
    }

    // std::vector<std::string_view> split_multi(const std::string_view& input, char delim)
    // {
    //     std::vector<std::string_view> out;

    //     // value: 3 shiny tomato bags, 2 light fuchsia bags, 1 bright salmon bag.
    //     //        012345679012345679012345679012345679012345679012345679012345679
    //     //                          21                     45
    //     //            {0, 21 -1 = 20}         (22, 45-22 = 23 -1 = 22)

    //     size_t start = 0;
    //     while (start < input.size())
    //     {
    //         size_t pos = input.find_first_of(delim, start);
    //         if (pos == std::string_view::npos)
    //             break;

    //         size_t substr_len = pos - start;
    //         out.push_back(std::string_view(&input[start], substr_len));
    //         start = pos + 2; // skip delim + whitespace after
    //     }

    //     size_t last_size = input.size() - start - 1; // remove last period
    //     out.push_back(std::string_view(&input[start], last_size));

    //     return out;
    // }
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
