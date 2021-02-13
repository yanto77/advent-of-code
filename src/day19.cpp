#include "advent2020.h"
#include <bitset>

namespace
{
    constexpr size_t RULE_N = 150;

    // Rule definitions, parsed from input
    //      0: 8 11              => [8, 11, 0, 0]
    //      1: 2 120 | 120 131   => [2, 120, 120, 131]
    //      2: "a"               => [0, 1, 0, 0]
    //      8: 42                => [42, 0, 0, 0]
    //      131: 2 | 120         => [2, 0, 120, 0]
    //
    //  - references have [0] as non-zero; pure value rules have [0] as 0 and value in [1]
    typedef std::array<uint8_t, 6> rule_def_t;
    typedef std::array<rule_def_t, RULE_N> rule_def_dict_t;

    // Rules parsed into DNF (r1 OR r2 OR ...)
    // Ref: https://en.wikipedia.org/wiki/Disjunctive_normal_form
    struct rule_t
    {
        std::bitset<256> mask;
        int8_t len;
    };
    typedef std::array<std::vector<rule_t>, RULE_N> rule_dnf_dict_t;

    rule_def_dict_t g_pre_rules {};
    rule_dnf_dict_t g_dnf_rules {};

    rule_t concat(const rule_t& r1, const rule_t& r2)
    {
        // auto len = static_cast<int8_t>(r1.len + r2.len);
        // static int max_len = 0;
        // if (len > max_len)
        // {
        //     max_len = len;
        //     printf("next max len: %d\n", max_len);
        // }

        return rule_t {
            .mask = append_bits(r1.mask, r2.mask, r2.len),
            .len = static_cast<int8_t>(r1.len + r2.len)
        };
    }

    template <typename T>
    void concat(std::vector<T>& v1, const std::vector<T>& v2)
    {
        v1.reserve(v1.size() + v2.size());
        for (auto val: v2)
            v1.emplace_back(val);
    }

    constexpr std::bitset<256> convert_line(const std::string_view& sv)
    {
        std::bitset<256> result;
        for (size_t i = 0; i < sv.size(); ++i)
            if (sv[i] == 'a')
                result.set(sv.size() - i - 1);

        // printf("converting line:\n");
        // std::cout << " - " << sv << std::endl;
        // printf(" - %d:", result); print_bits(result, true); printf("\n");

        return result;
    }

    /// Parse `line` into 6-int array `rule_def_t`.
    /// Returns {rule index, rule_def_t}.
    std::pair<size_t, rule_def_t> parse_line(const sv& line)
    {
        rule_def_t content {};
        size_t arr_idx = 0; // content index
        size_t rule_idx = 0; // rule index
        size_t ch_idx = 0; // line character index

        // Parse rule index
        {
            auto [num, end] = parse_num<uint8_t>(line, ch_idx);
            rule_idx = num;
            ch_idx += end + 2;
        }

        // Parse rule content
        if (line[ch_idx] == '"')
        {
            content[1] = (line[ch_idx + 1] == 'a');
        }
        else
        {
            while (ch_idx <= line.size())
            {
                if (line[ch_idx] == '|')
                {
                    arr_idx = 3;
                    ch_idx += 2;
                }
                else
                {
                    const auto& [num, end] = parse_num<uint8_t>(line, ch_idx);
                    content[arr_idx++] = num;
                    ch_idx = end + 1;

                    if (end == SIZE_MAX)
                        break;
                }
            }
        }

        return {rule_idx, content};
    }

    /// Constructs DNF representation of the rule and its subrules
    void construct_subrule(size_t idx)
    {
        if (!g_dnf_rules[idx].empty())
            return;

        const auto& rule = g_pre_rules[idx];
        // printf("    - constructing %zu: %d %d %d %d\n", idx, rule[0], rule[1], rule[2], rule[3]);

        if (rule[0] == 0) // rule is pure value rule
        {
            g_dnf_rules[idx].emplace_back(rule_t{.mask = rule[1], .len = 1});
        }
        else
        {
            for (int i: {0, 3})
            {
                // 0: 8 11              => [ 8,  11,   0,   0]
                // 1: 2 120 | 120 131   => [ 2, 120, 120, 131]
                // 8: 42                => [42,   0,   0,   0]
                // 131: 2 | 120         => [ 2,   0, 120,   0]

                if (rule[i] == 0) // single part rule, e.g. 8th
                    break;

                // printf("      - i %d %d\n", i, rule[i]);
                construct_subrule(rule[i]);

                if (rule[i+1] == 0 && rule[i+2] == 0)
                {
                    // one piece rule
                    concat(g_dnf_rules[idx], g_dnf_rules[rule[i]]);
                }
                else if (rule[i+1] != 0 && rule[i+2] == 0)
                {
                    // two AND-piece rule
                    construct_subrule(rule[i+1]);
                    for (auto p1: g_dnf_rules[rule[i]])
                        for (auto p2: g_dnf_rules[rule[i+1]])
                            g_dnf_rules[idx].emplace_back(concat(p1, p2));
                }
                else
                {
                    // three AND-piece rule
                    construct_subrule(rule[i+1]);
                    construct_subrule(rule[i+2]);
                    for (auto p1: g_dnf_rules[rule[i]])
                        for (auto p2: g_dnf_rules[rule[i+1]])
                            for (auto p3: g_dnf_rules[rule[i+2]])
                                g_dnf_rules[idx].emplace_back(concat(concat(p1, p2), p3));
                }
            }
        }
    }

    output_t evaluate(const input_t& input)
    {
        std::vector<std::bitset<256>> messages;
        g_pre_rules = {};
        g_dnf_rules = {};

        bool fill_rules = true;
        parse_input(input, [&](const sv& line)
        {
            if (line.empty())
            {
                fill_rules = false;
            }
            else if (fill_rules)
            {
                const auto& [rule_idx, content] = parse_line(line);
                g_pre_rules[rule_idx] = content;

                // printf("parsed: "); std::cout << line << std::endl;
                // printf("   -> %d %d %d %d %d %d\n", content[0], content[1], content[2], content[3], content[4], content[5]);
            }
            else // fill_nodes
            {
                messages.push_back(convert_line(line));
            }
        });

        // g_dnf_rules[0].emplace_back(rule_t{0, 0}); // allows handling [x, 0, y, 0] cases gracefully
        // for (size_t idx = 0; idx < g_pre_rules.size(); ++idx)
        // {
        //     construct_subrule(idx);
        // }

        construct_subrule(0);

        // printf("Rules:\n");
        // for (size_t i = 0; i < g_dnf_rules.size(); ++i)
        // {
        //     auto dnf_rule = g_dnf_rules[i];
        //     if (dnf_rule.empty())
        //         continue;

        //     // printf(" - %zu ", i);
        //     for (const auto& r: dnf_rule)
        //     {
        //         printf("[m%llu l%d], ", r.mask.to_ullong(), r.len);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        size_t part1 = 0;
        for (auto msg: messages)
        {
            // printf("- checking: "); print_bits(msg, true); printf("\n");
            for (auto dnf_part: g_dnf_rules[0])
            {
                // printf("       - r: "); print_bits(dnf_part.mask, true); printf("\n");
                if (msg == dnf_part.mask)
                {
                    ++part1;
                    // printf("         -> match OK!\n");
                    break;
                }
            }
        }

        printf("sizeof messages: %zu\n", sizeof(messages) + messages.capacity() * sizeof(messages[0]));
        printf("sizeof g_pre_rules: %zu\n", sizeof(g_pre_rules));

        size_t size = 0;
        for (auto rule: g_dnf_rules)
        {
            size += sizeof(rule) + rule.capacity() * sizeof(rule[0]);

            static size_t max_count = 0;
            if (rule.size() > max_count)
            {
                max_count = rule.size();
                printf("max array: %zu\n", max_count);
            }
        }
        printf("sizeof g_dnf_rules: %zu\n", sizeof(g_dnf_rules) + size);

        return {part1, 0};
    }
}

output_t day19(const input_t& input)
{
    return evaluate(input);
    // return {0, 0};
}

void day19_test()
{
    static_assert(append_bits<int>(0b010, 0b001, 2) == 0b1001);
    static_assert(append_bits<int>(0b010, 0b001, 1) == 0b101);

    char text1[100] =
        "0: 4 1 5\n"
        "1: 2 3 | 3 2\n"
        "2: 4 4 | 5 5\n"
        "3: 4 5 | 5 4\n"
        "4: \"a\"\n"
        "5: \"b\"\n"
        "\n"
        "ababbb\n"
        "bababa\n"
        "abbbab\n"
        "aaabbb\n"
        "aaaabbb\n";
    input_t test1 { text1, 100 };

    const auto& [part1, part2] = evaluate(test1);
    assert(part1 == 2);
}
