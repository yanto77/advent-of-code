#include "advent2020.h"

// rules for ticket fields,
// the numbers on your ticket,
// and the numbers on other nearby tickets

// Start by determining which tickets are completely invalid;
// these are tickets that contain values which aren't valid
// for any field. Ignore your ticket for now.

namespace
{
    struct range_t { uint16_t min; uint16_t max; };
    struct rule_t { sv type; range_t r1; range_t r2; };

    struct data_t
    {
        std::vector<uint16_t> my_ticket;
        std::vector<rule_t> rules;
        std::vector<std::vector<bool>> type_matrix; // (value in a ticket) x (rule)
    };

    struct parser_t
    {
        enum { RULES, MY_TICKET, OTHER_TICKETS } state = RULES;
        bool skip_next = false;
    };

    output_t evaluate(const input_t& input)
    {
        parser_t parser;
        data_t data;
        data.rules.reserve(50);

        output_t out;

        parse_input(input, [&](const sv& line)
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
                    case parser_t::RULES: { parser.state = parser_t::MY_TICKET; break; }
                    case parser_t::MY_TICKET:
                    {
                        parser.state = parser_t::OTHER_TICKETS;
                        break;
                    }
                    case parser_t::OTHER_TICKETS: { assert(false); break; }
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
                    data.rules.push_back({
                        .type = type,
                        .r1 = {to_int<uint16_t>(r1_min), to_int<uint16_t>(r1_max)},
                        .r2 = {to_int<uint16_t>(r2_min), to_int<uint16_t>(r2_max)}
                    });
                    break;
                }
                case parser_t::MY_TICKET:
                {
                    // Example: "7,1,14"
                    data.my_ticket = parse_ints<uint16_t>(line, ',');
                    break;
                }
                case parser_t::OTHER_TICKETS:
                {
                    // Example: "7,3,47"
                    for (uint16_t num: parse_ints<uint16_t>(line, ','))
                    {
                        bool passes_any = std::any_of(data.rules.begin(), data.rules.end(),
                            [&num](const rule_t& r) {
                                return (r.r1.min <= num && num <= r.r1.max) ||
                                       (r.r2.min <= num && num <= r.r2.max);
                            });

                        if (!passes_any)
                        {
                            out.part1 += num;
                        }
                    }

                    break;
                }
            }
        });

        return out;
    }
}

output_t day16(const input_t& input)
{
    // const auto& [part1, part2] = evaluate(input);
    // printf("part1: %zu, part2: %zu\n", part1, part2);
    // assert(part1 == 21956);
    return { 0, 0 };
}

void day16_test()
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
    //     input_t test1 { input, 128 };
    //     const auto& [part1, part2] = evaluate(test1);
    //     assert(part1 == 71);
    // }
    {
        char input2[118] =
            "class: 0-1 or 4-19\n"
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
        input_t test2 { input2, 118 };
        const auto& [part1, part2] = evaluate(test2);
        assert(part2 == 0);
    }
}
