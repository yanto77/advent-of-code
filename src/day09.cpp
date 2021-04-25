#include "advent2020.h"

namespace
{
    class solver_t
    {
        public:
            std::vector<int64_t> data;

        public:
            void parse(const input_t& in)
            {
                parse_input(in, [&](const sv& line)
                {
                    data.push_back(to_int<int64_t>(line));
                });
            }

            // Returns number in `input` that is not sum of any two numbers within
            // previous `sliding_n` numbers. If not found, returns -1;
            int64_t validate_input(size_t sliding_n)
            {
                for (size_t i = sliding_n; i < data.size(); ++i)
                {
                    bool found = false;
                    for (size_t wx = i - sliding_n; !found && wx < i; ++wx)
                    {
                        for (size_t wy = i - sliding_n; !found && wy < i && wx != wy; ++wy)
                        {
                            if (data[wx] + data[wy] == data[i])
                            {
                                found = true;
                            }
                        }
                    }

                    if (!found)
                    {
                        return data[i];
                    }
                }

                return -1;
            }

            std::pair<size_t, size_t> find_continuous_set(int64_t target_sum)
            {
                for (size_t i = 0; i < data.size(); ++i)
                {
                    int64_t sum = 0;
                    for (size_t j = i; j < data.size(); ++j)
                    {
                        sum += data[j];

                        if (sum == target_sum)
                            return { i, j };
                    }
                }

                return { 0, 0 };
            }

            std::pair<int64_t, int64_t> get_min_max(size_t start, size_t end)
            {
                int64_t min = INT64_MAX;
                int64_t max = INT64_MIN;

                for (size_t i = start; i < end; ++i)
                {
                    if (data[i] > max) max = data[i];
                    if (data[i] < min) min = data[i];
                }

                return { min, max };
            }
    };
}

output_t day09(const input_t& input)
{
    solver_t solver;
    solver.parse(input);

    size_t invalid_num = solver.validate_input(25);

    const auto& [start, end] = solver.find_continuous_set(invalid_num);
    const auto& [min, max] = solver.get_min_max(start, end);

    return { invalid_num, static_cast<size_t>(min + max) };
}

void day09_test()
{
    char text[] = "35\n"
                  "20\n"
                  "15\n"
                  "25\n"
                  "47\n"
                  "40\n"
                  "62\n"
                  "55\n"
                  "65\n"
                  "95\n"
                  "102\n"
                  "117\n"
                  "150\n"
                  "182\n"
                  "127\n"
                  "219\n"
                  "299\n"
                  "277\n"
                  "309\n"
                  "576\n";
    input_t input { text, sizeof(text) };

    solver_t solver;
    solver.parse(input);

    assert(solver.data[0] == 35);
    assert(solver.data[19] == 576);
    assert(solver.validate_input(5) == 127);

    const auto& [start, end] = solver.find_continuous_set(127);
    const auto& [min, max] = solver.get_min_max(start, end);
    assert(start == 2 && end == 5);
    assert(min == 15 && max == 47);
}
