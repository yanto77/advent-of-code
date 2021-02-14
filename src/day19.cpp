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
    typedef std::array<uint8_t, 4> rule_def_t;
    typedef std::array<rule_def_t, RULE_N> rule_def_dict_t;

    // Rules parsed into DNF (r1 OR r2 OR ...), where {r1, r2} are rule_t
    // Ref: https://en.wikipedia.org/wiki/Disjunctive_normal_form
    struct rule_t 
    { 
        uint8_t mask;
        int8_t len; 
    };
    typedef std::array<std::vector<rule_t>, RULE_N> rule_dnf_dict_t;

    rule_def_dict_t g_pre_rules {};
    rule_dnf_dict_t g_dnf_rules {};

    rule_t concat(const rule_t& r1, const rule_t& r2)
    {
        return rule_t { 
            .mask = append_bits(r1.mask, r2.mask, r2.len), 
            .len = static_cast<int8_t>(r1.len + r2.len)
        };
    }

    /// Parse `line` into 6-int array `rule_def_t`.
    /// Returns {rule index, rule_def_t}.
    std::pair<size_t, rule_def_t> parse_line(const sv& line)
    {
        std::pair<size_t, rule_def_t> result {};

        size_t ch_idx = 0; // line character index

        /// Parse rule index
        auto [num, end] = parse_num<uint8_t>(line, ch_idx);
        result.first = num;
        ch_idx += end + 2;

        /// Parse rule content
        if (line[ch_idx] == '"')
        {
            result.second[1] = (line[ch_idx + 1] == 'a');
        }
        else
        {
            size_t arr_idx = 0; // content index
            while (ch_idx <= line.size())
            {
                if (line[ch_idx] == '|')
                {
                    arr_idx = 2;
                    ch_idx += 2;
                }
                else
                {
                    const auto& [num, end] = parse_num<uint8_t>(line, ch_idx);
                    result.second[arr_idx++] = num;
                    ch_idx = end + 1;

                    if (end == SIZE_MAX)
                        break;
                }
            }
        }

        return result;
    }

    /// Constructs DNF representation of the rule and its subrules
    void construct_subrule(size_t idx)
    {
        if (!g_dnf_rules[idx].empty())
            return;

        const auto& rule = g_pre_rules[idx];
        if (rule[0] == 0) // rule is pure value rule
        {
            g_dnf_rules[idx].emplace_back(rule_t{.mask = rule[1], .len = 1});
        }
        else
        {
            for (int i: {0, 2})
            {
                // 0: 8 11              => [ 8,  11,   0,   0]
                // 1: 2 120 | 120 131   => [ 2, 120, 120, 131]
                // 8: 42                => [42,   0,   0,   0]
                // 131: 2 | 120         => [ 2,   0, 120,   0]

                if (rule[i] == 0) // single part rule, e.g. 8th
                    break;

                construct_subrule(rule[i]);
                construct_subrule(rule[i+1]);

                for (auto p1: g_dnf_rules[rule[i]])
                    for (auto p2: g_dnf_rules[rule[i+1]])
                        g_dnf_rules[idx].emplace_back(concat(p1, p2));
            }
        }
    }

    std::bitset<256> get_as_bitset(size_t idx)
    {
        std::bitset<256> mask;
        for (const rule_t& rule_part: g_dnf_rules[idx])
            mask.set(rule_part.mask);

        return mask;
    }

    output_t evaluate(const input_t& input)
    {
        std::vector<sv> messages;
        g_pre_rules = {};
        g_dnf_rules = {};

        bool fill_rules = true;
        parse_input(input, [&](const sv& line)
        {
            if (line.empty()) fill_rules = false;
            
            else if (fill_rules)
            {
                const auto& [rule_idx, content] = parse_line(line);
                g_pre_rules[rule_idx] = content;
            }
            else // fill_nodes
            {
                messages.push_back(line);
            }
        });

        // The actual 0th rule is processed implicitly afterwards. This allows
        // handling [x, 0, y, 0] cases gracefully.
        g_dnf_rules[0].emplace_back(rule_t{0, 0}); 

        construct_subrule(42);
        construct_subrule(31);
        std::bitset<256> rule42 = get_as_bitset(42);
        std::bitset<256> rule31 = get_as_bitset(31);

        size_t part1 = 0; // messages matching rule 0: [42 42 31]
        for (const sv& msg: messages)
        {
            int rule42_match = 0, rule31_match = 0;
            for (size_t ch = 0; ch < msg.size(); ch += 8)
            {
                uint8_t result = 0;
                for (size_t bit = 0; bit < 8; ++bit)
                    if (msg[ch + bit] == 'a')
                        set_bit(result, 7 - bit);

                if (rule42.test(result)) ++rule42_match;
                if (rule31.test(result)) ++rule31_match;
            }
            
            if (rule42_match == 2 && rule31_match == 1)
                ++part1;
        }

        return {part1, 0};
    }
}

output_t day19(const input_t& input)
{
    return evaluate(input);
}

void day19_test()
{
    static_assert(append_bits<int>(0b010, 0b001, 2) == 0b1001);
    static_assert(append_bits<int>(0b010, 0b001, 1) == 0b101);
}
