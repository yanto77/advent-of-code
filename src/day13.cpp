#include "advent2020.h"

struct data_t
{
    int t0 = 0;
    std::vector<int> busses = {};
};

namespace
{
    data_t parse_input(const input_t& input)
    {
        data_t parsed;
        parsed.busses.reserve(100);

        bool first = true;
        parse_input(input, [&](const sv& line)
        {
            if (first)
            {
                parsed.t0 = to_int<int>(line);
                first = false;
            }
            else
            {
                for (const sv& bus: split_multi(line, ','))
                    if (bus.data()[0] != 'x')
                        parsed.busses.push_back(to_int<int>(bus));
            }
        });

        return parsed;
    }

    std::pair<int, int> evaluate(const data_t& data)
    {
        int k = 2;
        int n = static_cast<int>(data.busses.size());
        std::vector<int> result { n, 0 };

        while (true)
        {
            // Apply
            for (int i = 0; i < n; ++i)
                result[i] = k * data.busses[i];

            // Check
            std::pair<int, int> min { INT32_MAX, 0 }; // { value, bus ID }
            for (int i = 0; i < n; ++i)
                if (data.t0 < result[i] && result[i] < min.first)
                    min = { result[i], i };

            if (min.first != INT32_MAX)
            {
                // printf("Found min for bus %d, t1 %d, t1-t0: %d\n", data.busses[min.second], min.first, min.first - data.t0);
                printf("k: %d\n", k);
                return {(min.first - data.t0), data.busses[min.second]};
            }

            ++k;
        }

        assert(false);
        return {0, 0};
    }
}


output_t day13(const input_t& input)
{
    data_t data = parse_input(input);

    // find 939 + x = k * y, where y is one of busses
    const auto& [diff, bus] = evaluate(data);
    printf("diff: %d, bus: %d, result: %d\n", diff, bus, diff*bus);

    return { 0, 0 };
}

void day13_test()
{
    char text[25] = "939\n7,13,x,x,59,x,31,19\n";
    input_t test1 { text, 25 };

    data_t data = parse_input(test1);
    assert(data.t0 == 939);
    assert(data.busses == std::vector<int>({7, 13, 59, 31, 19}));

    // find 939 + x = k * y, where y is one of busses
    const auto& [diff, bus] = evaluate(data);
    // printf("diff: %d, bus: %d, result: %d\n", diff, bus, diff*bus);
    assert(diff == 5 && bus == 59);
}
