#include "advent2020.h"
#include "setup/days.h"
#include <chrono>

static std::vector<advent_t> g_advent2020 = 
{
    // { "Day 01", "input/day01.txt", day01, day01_test },
    // { "Day 02", "input/day02.txt", day02, day02_test },
    // { "Day 03", "input/day03.txt", day03, day03_test },
    // { "Day 04", "input/day04.txt", day04, day04_test },
    // { "Day 05", "input/day05.txt", day05, day05_test },
    // { "Day 06", "input/day06.txt", day06, day06_test },
    // { "Day 07", "input/day07.txt", day07, day07_test },
    // { "Day 08", "input/day08.txt", day08, day08_test },
    { "Day 09", "input/day09.txt", day09, day09_test },
    // { "Day 10", "input/day10.txt", day10, day10_test },
    // { "Day 11", "input/day11.txt", day11, day11_test },
    // { "Day 12", "input/day12.txt", day12, day12_test },
    // { "Day 13", "input/day13.txt", day13, day13_test },
    // { "Day 14", "input/day14.txt", day14, day14_test },
    // { "Day 15", "input/day15.txt", day15, day15_test },
    // { "Day 16", "input/day16.txt", day16, day16_test },
    // { "Day 17", "input/day17.txt", day17, day17_test },
    // { "Day 18", "input/day18.txt", day18, day18_test },
    // { "Day 19", "input/day19.txt", day19, day19_test },
    // { "Day 20", "input/day20.txt", day20, day20_test },
    // { "Day 21", "input/day21.txt", day21, day21_test },
    // { "Day 22", "input/day22.txt", day22, day22_test },
    // { "Day 23", "input/day23.txt", day23, day23_test },
    // { "Day 24", "input/day24.txt", day24, day24_test },
    // { "Day 25", "input/day25.txt", day25, day25_test },
};

namespace
{
    double measure(const advent_t& day)
    {
        input_t input;
        load_input(input, day.input_file);

        auto t0 = std::chrono::steady_clock::now();
        (*day.fn_exec)(input);
        auto elapsed = std::chrono::steady_clock::now() - t0;

        free_input(input);

        return elapsed.count();
    }
}

int main()
{
    double total_time = 0;

    for (const auto& A : g_advent2020)
    {
        printf("%s: ", A.day_id.c_str());

        A.fn_test();

        double day_time = measure(A);

        printf(" [%ld μs]\n", int64_t(day_time * 1e-3));
        total_time += day_time;
    }

    printf("Total: %ld μs\n", int64_t(total_time * 1e-3));

    return 0;
}
