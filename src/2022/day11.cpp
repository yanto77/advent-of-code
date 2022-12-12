#include "setup/all.h"

ADVENT_DAY(2022, 11, 58786, 14952185856);

namespace 
{
    enum class operator_t { ADD, MUL, POW };

    struct monkey_t 
    { 
        std::vector<uint64_t> items; 
        operator_t func = operator_t::ADD;
        uint64_t value = 0;
        uint64_t div = 1;
        size_t pass = 0; 
        size_t fail = 0;
    };

    std::array<monkey_t, 8> parse_monkeys(str_view input)
    {
        std::array<monkey_t, 8> monkeys;
        int32_t monkeyIdx = -1;

        for_each_split(input, '\n', [&](str_view line)
        {
            if (line.starts_with("Monkey"))
            {
                monkeyIdx++;
            }
            else if (line.starts_with("  Starting items: "))
            {
                constexpr size_t skip = sizeof("  Starting items: ") - 1;
                const str_view numbers { &line[skip], line.size() - skip };
                parse_uint_scalar(numbers, [&](uint64_t num)
                {
                    monkeys[monkeyIdx].items.push_back(num);
                });
            }
            else if (line.starts_with("  Operation: "))
            {
                if (line[25] == 'o')
                {
                    monkeys[monkeyIdx].func = operator_t::POW;
                    monkeys[monkeyIdx].value = 2;
                }
                else if (line[23] == '*')
                {
                    auto num = to_int<uint32_t>(str_view { &line[25], line.size() - 25 });
                    monkeys[monkeyIdx].func = operator_t::MUL;
                    monkeys[monkeyIdx].value = num;
                }
                else if (line[23] == '+')
                {
                    auto num = to_int<uint32_t>(str_view { &line[25], line.size() - 25 });
                    monkeys[monkeyIdx].func = operator_t::ADD;
                    monkeys[monkeyIdx].value = num;
                }
            }
            else if (line.starts_with("  Test: "))
            {
                constexpr size_t skip = sizeof("  Test: divisible by ") - 1;
                auto num = to_int<uint32_t>(str_view { &line[skip], line.size() - skip });
                monkeys[monkeyIdx].div = num;
            }
            else if (line.starts_with("    If "))
            {
                if (line[7] == 't') // "true"
                {
                    constexpr size_t skip = sizeof("    If true: throw to monkey ") - 1;
                    const char num = line[skip];
                    monkeys[monkeyIdx].pass = parse_uint(num);
                }
                else // "false"
                {
                    constexpr size_t skip = sizeof("    If false: throw to monkey ") - 1;
                    const char num = line[skip];
                    monkeys[monkeyIdx].fail = parse_uint(num);
                }
            }
        });

        return monkeys;
    }

    size_t simulate(std::array<monkey_t, 8> monkeys, size_t max_rounds, bool part1)
    {
        std::array<size_t, 8> counters {};

        // The common mod is used to keep the numbers small enough so that they 
        // don't overflow. Because it's LCM, it doesn't affect monkey dynamics.
        // NB: all inputs are primes, so LCM is a product of them.
        uint64_t common_mod = 1;
        for (auto monkey: monkeys)
        {
            common_mod *= monkey.div;
        }

        for (size_t round = 0; round < max_rounds; round++)
        {
            for (size_t idx = 0; idx < 8; idx++)
            {
                monkey_t& m = monkeys[idx];

                for (uint64_t item: m.items)
                {
                    counters[idx]++;

                    switch (m.func)
                    {
                        case operator_t::ADD: { item += m.value; break; }
                        case operator_t::MUL: { item *= m.value; break; }
                        case operator_t::POW: { item = binpow(item, m.value); break; }
                    }
                    
                    if (part1)
                        item /= 3;
                    else
                        item %= common_mod;

                    size_t next_idx = (item % m.div == 0) ? m.pass : m.fail;
                    monkeys[next_idx].items.push_back(item);
                }

                m.items.clear();
            }
        }

        std::sort(counters.begin(), counters.end());
        return counters[6] * counters[7];
    }
}

result_t Day_2022_11::run_solution(str_view input) const
{
    std::array<monkey_t, 8> monkeys = parse_monkeys(input);
    size_t part1 = simulate(monkeys, 20, true);
    size_t part2 = simulate(monkeys, 10000, false);
    return { part1, part2 };
}

void Day_2022_11::run_tests() const
{
}
