#include "setup/all.h"

ADVENT_DAY(2020, 19, 208, 316);

namespace
{
    struct rule_t
    {
        uint8_t mask;
        int8_t len;

        friend rule_t concat(const rule_t& r1, const rule_t& r2)
        {
            return rule_t { 
                .mask = append_bits(r1.mask, r2.mask, r2.len), 
                .len = static_cast<int8_t>(r1.len + r2.len)
            };
        }
    };

    class solver
    {
        // Rules parsed into DNF (r1 OR r2 OR ...), where {r1, r2} are rule_t
        // Ref: https://en.wikipedia.org/wiki/Disjunctive_normal_form
        constexpr static size_t RULE_N = 150;
        std::array<std::vector<rule_t>, RULE_N> m_dnf_rules {};
        std::vector<str_view> m_messages {};

      private:
        /// Parse `line` into 4-int array `rule_def_t`.
        /// Returns {rule index, std::array<uint8_t, 4>}.
        std::pair<size_t, std::array<uint8_t, 4>> parse_line(str_view line) const
        {
            std::pair<size_t, std::array<uint8_t, 4>> result {};

            size_t ch_idx = 0; // line character index

            /// Parse rule index
            auto [num, end] = to_int<uint8_t>(line, ch_idx);
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
                        const auto& [num, end] = to_int<uint8_t>(line, ch_idx);
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
        void construct_subrule(const std::array<std::array<uint8_t, 4>, RULE_N>& rules, size_t idx)
        {
            if (!m_dnf_rules[idx].empty())
                return;

            const auto& rule = rules[idx];
            if (rule[0] == 0) // rule is pure value rule
            {
                m_dnf_rules[idx].emplace_back(rule_t { .mask = rule[1], .len = 1 });
            }
            else
            {
                for (int i : { 0, 2 })
                {
                    // 0: 8 11              => [ 8,  11,   0,   0]
                    // 1: 2 120 | 120 131   => [ 2, 120, 120, 131]
                    // 8: 42                => [42,   0,   0,   0]
                    // 131: 2 | 120         => [ 2,   0, 120,   0]

                    if (rule[i] == 0) // single part rule, e.g. 8th
                        break;

                    construct_subrule(rules, rule[i]);
                    construct_subrule(rules, rule[i + 1]);

                    for (auto p1 : m_dnf_rules[rule[i]])
                        for (auto p2 : m_dnf_rules[rule[i + 1]])
                            m_dnf_rules[idx].emplace_back(concat(p1, p2));
                }
            }
        }

        std::bitset<256> get_as_bitset(size_t idx) const
        {
            std::bitset<256> mask;
            for (const rule_t& rule_part : m_dnf_rules[idx])
                mask.set(rule_part.mask);

            return mask;
        }

      public:
        void parse(str_view input)
        {
            // Rule definitions, parsed from input
            //      0: 8 11              => [8, 11, 0, 0]
            //      1: 2 120 | 120 131   => [2, 120, 120, 131]
            //      2: "a"               => [0, 1, 0, 0]
            //      8: 42                => [42, 0, 0, 0]
            //      131: 2 | 120         => [2, 0, 120, 0]
            //
            //  - references have [0] as non-zero; pure value rules have [0] as 0 and value in [1]
            std::array<std::array<uint8_t, 4>, RULE_N> rules {};

            bool fill_rules = true;
            for_each_split(input, '\n', [&](str_view line)
            {
                if (line.empty()) fill_rules = false;

                else if (fill_rules)
                {
                    const auto& [rule_idx, content] = parse_line(line);
                    rules[rule_idx] = content;
                }
                else // fill_nodes
                {
                    m_messages.push_back(line);
                }
            });

            // The actual 0th rule is processed implicitly afterwards. This allows
            // handling [x, 0, y, 0] cases gracefully.
            m_dnf_rules[0].emplace_back(rule_t { 0, 0 });

            construct_subrule(rules, 42);
            construct_subrule(rules, 31);
        }

        result_t solve()
        {
            std::bitset<256> r42 = get_as_bitset(42);
            std::bitset<256> r31 = get_as_bitset(31);

            const size_t len = m_dnf_rules[42].front().len;

            // pt1: match: 0=(42 42 31)
            size_t part1 = 0;
            // pt2: match: 0=(8 11), 8=(42 | 42 8), 11=(42 31 | 42 11 31)
            size_t part2 = 0;

            for (str_view msg : m_messages)
            {
                bool fail = false;
                int r42_match = 0, r31_match = 0;

                for (size_t ch = 0; ch < msg.size(); ch += len)
                {
                    uint8_t result = 0;
                    for (size_t bit = 0; bit < len; ++bit)
                        if (msg[ch + bit] == 'a')
                            set_bit(result, len - 1 - bit);

                    if (r42.test(result) && r31_match != 0)
                        fail = true;

                    if (r42.test(result)) ++r42_match;
                    if (r31.test(result)) ++r31_match;
                }

                if (fail)
                    continue;

                part1 += static_cast<int>(r42_match == 2 && r31_match == 1);
                part2 += static_cast<int>(r42_match > r31_match && r31_match > 0);
            }

            return { part1, part2 };
        }
    };
}

result_t Day_2020_19::run_solution(str_view input) const
{
    solver s;
    s.parse(input);
    return s.solve();
}

void Day_2020_19::run_tests() const
{
    static_assert(append_bits<int>(0b010, 0b001, 2) == 0b1001);
    static_assert(append_bits<int>(0b010, 0b001, 1) == 0b101);

    char text1[] = "42: 9 14 | 10 1\n"
                   "9: 14 27 | 1 26\n"
                   "10: 23 14 | 28 1\n"
                   "1: \"a\"\n"
                   "11: 42 31\n"
                   "5: 1 14 | 15 1\n"
                   "19: 14 1 | 14 14\n"
                   "12: 24 14 | 19 1\n"
                   "16: 15 1 | 14 14\n"
                   "31: 14 17 | 1 13\n"
                   "6: 14 14 | 1 14\n"
                   "2: 1 24 | 14 4\n"
                   "0: 8 11\n"
                   "13: 14 3 | 1 12\n"
                   "15: 1 | 14\n"
                   "17: 14 2 | 1 7\n"
                   "23: 25 1 | 22 14\n"
                   "28: 16 1\n"
                   "4: 1 1\n"
                   "20: 14 14 | 1 15\n"
                   "3: 5 14 | 16 1\n"
                   "27: 1 6 | 14 18\n"
                   "14: \"b\"\n"
                   "21: 14 1 | 1 14\n"
                   "25: 1 1 | 1 14\n"
                   "22: 14 14\n"
                   "8: 42\n"
                   "26: 14 22 | 1 20\n"
                   "18: 15 15\n"
                   "7: 14 5 | 1 21\n"
                   "24: 14 1\n"
                   "\n"
                   "abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa\n"
                   "bbabbbbaabaabba\n"
                   "babbbbaabbbbbabbbbbbaabaaabaaa\n"
                   "aaabbbbbbaaaabaababaabababbabaaabbababababaaa\n"
                   "bbbbbbbaaaabbbbaaabbabaaa\n"
                   "bbbababbbbaaaaaaaabbababaaababaabab\n"
                   "ababaaaaaabaaab\n"
                   "ababaaaaabbbaba\n"
                   "baabbaaaabbaaaababbaababb\n"
                   "abbbbabbbbaaaababbbbbbaaaababb\n"
                   "aaaaabbaabaaaaababaa\n"
                   "aaaabbaaaabbaaa\n"
                   "aaaabbaabbaaaaaaabbbabbbaaabbaabaaa\n"
                   "babaaabbbaaabaababbaabababaaab\n"
                   "aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba\n"
                   "\n";

    solver s;
    s.parse(text1);
    auto out = s.solve();
    assert(out.part1 == 3 && out.part2 == 12);
}
