#include "setup/all.h"

ADVENT_DAY(2020, 16, 21956, 3709435214239);

namespace
{
    [[maybe_unused]]
    void print(const std::vector<std::vector<bool>>& matrix)
    {
        const size_t ydim = matrix.size();
        const size_t xdim = matrix[0].size();

        for (size_t y = 0; y < ydim; ++y)
        {
            for (size_t x = 0; x < xdim; ++x)
            {
                if (matrix[y][x] == 1)
                    printf(COLOR_GREEN() "%d" COLOR_RESET(), matrix[y][x]);
                else
                    printf(COLOR_RED() "%d" COLOR_RESET(), matrix[y][x]);
            }
            printf("\n");
        }
    }
}

namespace
{
    struct range_t
    {
        uint16_t min;
        uint16_t max;
    };
    struct rule_t
    {
        str_view type;
        range_t r1;
        range_t r2;
        size_t index; // index from the ticket values

        bool pass(uint16_t num) const
        {
            return (r1.min <= num && num <= r1.max) ||
                   (r2.min <= num && num <= r2.max);
        }
    };

    struct data_t
    {
        std::vector<uint16_t> my_ticket;
        std::vector<rule_t> rules;
        std::vector<std::vector<bool>> type_matrix; // ydim - ticket values, xdim - rules

        void simplify_matrix()
        {
            const size_t ydim = my_ticket.size();
            const size_t xdim = rules.size();

            // print(type_matrix);

            std::vector<std::vector<bool>> clear_copy = { ydim, std::vector<bool>(xdim, false) };

            while (true)
            {
                ssize_t row = -1;
                for (size_t y = 0; y < ydim; ++y)
                {
                    if (count_bits(type_matrix[y]) == 1)
                    {
                        row = y;
                        break;
                    }
                }

                if (row == -1)
                {
                    // when no single bit rows remain, the solution has been found.
                    // alternatively there is no solution, but that doesn't happen with given input.
                    break;
                }
                else
                {
                    clear_copy[row] = type_matrix[row];
                    for (size_t y = 0; y < ydim; ++y)
                    {
                        reset_bitmask(type_matrix[y], clear_copy[row]);
                    }
                }
            }

            // copy clear state to actual data
            type_matrix = clear_copy;

            // print(type_matrix);
        }

        void save_rule_indexes()
        {
            const size_t ydim = my_ticket.size();
            const size_t xdim = rules.size();
            for (size_t y = 0; y < ydim; ++y)
                for (size_t x = 0; x < xdim; ++x)
                    if (type_matrix[y][x] == 1)
                        rules[x].index = y;
        }
    };

    struct parser_t
    {
        enum { RULES, MY_TICKET, OTHER_TICKETS } state = RULES;
        bool skip_next = false;
    };

    output_t evaluate(str_view input)
    {
        parser_t parser;
        data_t data;
        data.rules.reserve(25);

        output_t out;

        parse_input(input, [&](str_view line)
        {
            // skip unnecessary lines
            if (parser.skip_next)
            {
                parser.skip_next = false;
                return;
            }

            // change parser state
            if (line == "")
            {
                parser.skip_next = true;
                switch (parser.state)
                {
                    case parser_t::RULES:
                    {
                        parser.state = parser_t::MY_TICKET;
                        break;
                    }
                    case parser_t::MY_TICKET:
                    {
                        parser.state = parser_t::OTHER_TICKETS;
                        const size_t ydim = data.my_ticket.size();
                        const size_t xdim = data.rules.size();
                        data.type_matrix = { ydim, std::vector<bool>(xdim, true) };
                        break;
                    }
                    case parser_t::OTHER_TICKETS:
                    {
                        assert(false);
                        break;
                    }
                }
                return;
            }

            // process the actual data
            switch (parser.state)
            {
                case parser_t::RULES:
                {
                    // Example: "arrival platform: 40-831 or 837-961"
                    const auto& [type, ranges] = split_single(line, ": ");
                    const auto& [range1, range2] = split_single(ranges, " or ");
                    const auto& [r1_min, r1_max] = split_single(range1, "-");
                    const auto& [r2_min, r2_max] = split_single(range2, "-");
                    data.rules.push_back({ .type = type,
                                           .r1 = { to_int<uint16_t>(r1_min), to_int<uint16_t>(r1_max) },
                                           .r2 = { to_int<uint16_t>(r2_min), to_int<uint16_t>(r2_max) } });
                    break;
                }
                case parser_t::MY_TICKET:
                {
                    // Example: "7,1,14"
                    data.my_ticket = to_multi_int<uint16_t>(line);
                    break;
                }
                case parser_t::OTHER_TICKETS:
                {
                    // Example: "7,3,47"
                    const auto& other_ticket = to_multi_int<uint16_t>(line);

                    const size_t ydim = data.my_ticket.size();
                    const size_t xdim = data.rules.size();
                    for (size_t y = 0; y < ydim; ++y)
                    {
                        const uint16_t num = other_ticket[y];
                        bool passes_any = std::any_of(data.rules.begin(), data.rules.end(),
                            [&num](const rule_t& r) { return r.pass(num); });

                        if (!passes_any) // invalid ticket
                        {
                            out.part1 += num;
                        }
                        else // valid ticket
                        {
                            for (size_t x = 0; x < xdim; ++x)
                            {
                                if (!data.rules[x].pass(num))
                                {
                                    data.type_matrix[y][x] = false;
                                }
                            }
                        }
                    }

                    break;
                }
            }
        });

        data.simplify_matrix();
        data.save_rule_indexes();

        out.part2 = 1;
        for (const auto& rule : data.rules)
        {
            if (rule.type.starts_with("departure"))
            {
                out.part2 *= data.my_ticket[rule.index];
            }
        }

        return out;
    }
}

output_t Day_2020_16::run_solution(str_view input) const
{
    return evaluate(input);
}

void Day_2020_16::run_tests() const
{
    // {
    //     char input[128] =
    //         "class: 1-3 or 5-7\n"
    //         "row: 6-11 or 33-44\n"
    //         "seat: 13-40 or 45-50\n"
    //         "\n"
    //         "your ticket:\n"
    //         "7,1,14\n"
    //         "\n"
    //         "nearby tickets:\n"
    //         "7,3,47\n"
    //         "40,4,50\n"
    //         "55,2,20\n"
    //         "38,6,12\n";
    //     const auto& [part1, part2] = evaluate(input);
    //     assert(part1 == 71);
    // }
    {
        char input2[] = "class: 0-1 or 4-19\n"
                        "row: 0-5 or 8-19\n"
                        "seat: 0-13 or 16-19\n"
                        "\n"
                        "your ticket:\n"
                        "11,12,13\n"
                        "\n"
                        "nearby tickets:\n"
                        "3,9,18\n"
                        "15,1,5\n"
                        "5,14,9\n";
        const auto& [part1, part2] = evaluate(input2);
        assert(part2 == 1);
    }
}
