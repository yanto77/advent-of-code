#include "setup/all.h"

ADVENT_DAY(2020, 9, 507622668, 76688505);

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

            // Return if a number at index `tail` is valid.
            // Validity means that the number can be constructed by summing any
            // two numbers from N values before index `tail` (where N is `sliding_n`)
            bool is_valid_num(size_t tail, size_t sliding_n)
            {
                size_t head = tail - sliding_n;
                for (size_t wx = head; wx < tail; ++wx)
                    for (size_t wy = head; wy < wx; ++wy)
                        if (data[wx] + data[wy] == data[tail])
                            return true;

                return false;
            }

            // Returns number in `input` that is not valid
            // If not found, returns -1;
            int64_t get_invalid_num(size_t sliding_n)
            {
                for (size_t i = sliding_n; i < data.size(); ++i)
                    if (!is_valid_num(i, sliding_n))
                        return data[i];

                return -1;
            }

            std::pair<size_t, size_t> find_continuous_set(int64_t target_sum)
            {
                size_t tail = 0;
                size_t head = 1;
                int64_t sum = data[tail] + data[head];

                while (true) // nb. assumes the result is _always_ found within bounds
                {
                    if (sum < target_sum)
                        sum += data[++head];
                    else if (sum > target_sum)
                        sum -= data[tail++];
                    else break; // equality
                }

                return { tail, head };
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

output_t Day_2020_9::run_solution(const input_t& input) const
{
    solver_t solver;
    solver.parse(input);

    size_t invalid_num = solver.get_invalid_num(25);

    const auto& [start, end] = solver.find_continuous_set(invalid_num);
    const auto& [min, max] = solver.get_min_max(start, end);

    return { invalid_num, static_cast<size_t>(min + max) };
}

void Day_2020_9::run_tests() const
{
    char text[] = "35\n20\n15\n25\n47\n40\n62\n55\n65\n95\n102\n"
                  "117\n150\n182\n127\n219\n299\n277\n309\n576\n";
    input_t input { text, sizeof(text) };

    solver_t solver;
    solver.parse(input);

    assert(solver.data[0] == 35);
    assert(solver.data[19] == 576);
    assert(solver.get_invalid_num(5) == 127);

    const auto& [start, end] = solver.find_continuous_set(127);
    const auto& [min, max] = solver.get_min_max(start, end);
    assert(start == 2 && end == 5);
    assert(min == 15 && max == 47);
}
