#include "advent2020.h"
#include "setup/days.h"
#include <chrono>

struct advent_t
{
    std::string day_id;
    std::string input_file;

    output_t (*fn_exec)(const input_t&);
    void (*fn_test)();

    output_t answers;
};

static std::vector<advent_t> g_advent2020 =
{
    // advent_t{ "Day 01", "input/day01.txt", day01, day01_test, output_t{ 1010884, 253928438 }},
    // advent_t{ "Day 02", "input/day02.txt", day02, day02_test, output_t{ 465, 294 }},
    // advent_t{ "Day 03", "input/day03.txt", day03, day03_test, output_t{ 268, 3093068400 }},
    // advent_t{ "Day 04", "input/day04.txt", day04, day04_test, output_t{ 237, 172 }},
    // advent_t{ "Day 05", "input/day05.txt", day05, day05_test, output_t{ 953, 615 }},
    // advent_t{ "Day 06", "input/day06.txt", day06, day06_test, output_t{ 6947, 3398 }},
    // advent_t{ "Day 07", "input/day07.txt", day07, day07_test, output_t{ 172, 39645 }},
    // advent_t{ "Day 08", "input/day08.txt", day08, day08_test, output_t{ 1949, 2092 }},
    // advent_t{ "Day 09", "input/day09.txt", day09, day09_test, output_t{ 507622668, 76688505 }},
    // advent_t{ "Day 10", "input/day10.txt", day10, day10_test, output_t{ 2664, 148098383347712 }},
    // advent_t{ "Day 11", "input/day11.txt", day11, day11_test, output_t{ 2108, 1897 }},
    // advent_t{ "Day 12", "input/day12.txt", day12, day12_test, output_t{ 364, 39518 }},
    // advent_t{ "Day 13", "input/day13.txt", day13, day13_test, output_t{ 2947, 526090562196173 }},
    // advent_t{ "Day 14", "input/day14.txt", day14, day14_test, output_t{ 14954914379452, 3415488160714 }},
    // advent_t{ "Day 15", "input/day15.txt", day15, day15_test, output_t{ 1294, 573522 }},
    // advent_t{ "Day 16", "input/day16.txt", day16, day16_test, output_t{ 21956, 3709435214239 }},
    // advent_t{ "Day 17", "input/day17.txt", day17, day17_test, output_t{ 401, 2224 }},
    // advent_t{ "Day 18", "input/day18.txt", day18, day18_test, output_t{ 4696493914530, 362880372308125 }},
    // advent_t{ "Day 19", "input/day19.txt", day19, day19_test, output_t{ 208, 316 }},
    advent_t{ "Day 20", "input/day20.txt", day20, day20_test, output_t{ 0, 0 }},
    // advent_t{ "Day 21", "input/day21.txt", day21, day21_test, output_t{ 0, 0 }},
    // advent_t{ "Day 22", "input/day22.txt", day22, day22_test, output_t{ 0, 0 }},
    // advent_t{ "Day 23", "input/day23.txt", day23, day23_test, output_t{ 0, 0 }},
    // advent_t{ "Day 24", "input/day24.txt", day24, day24_test, output_t{ 0, 0 }},
    // advent_t{ "Day 25", "input/day25.txt", day25, day25_test, output_t{ 0, 0 }},
};

int main()
{
    double total_time = 0;

    for (const auto& A : g_advent2020)
    {
        // Run some initial tests
        A.fn_test();

        // Run the actual task with actual input
        input_t input = load_input(A.input_file);
        auto t0 = std::chrono::steady_clock::now();
        output_t out = (*A.fn_exec)(input);
        auto elapsed = (std::chrono::steady_clock::now() - t0);
        double time = (elapsed.count() * 1e-3);
        free_input(input);

        // Output
        if (out.part1 == 0 || out.part2 == 0)
        {
            printf(COLOR_YELLOW() "%s: TODO [%5ld μs]\n" COLOR_RESET(), A.day_id.c_str(), int64_t(time));
            printf("  - Expected: pt1 = %zu, pt2 = %zu\n", A.answers.part1, A.answers.part2);
            printf("  - Actual  : pt1 = %zu, pt2 = %zu\n", out.part1, out.part2);
        }
        else if (out.part1 == A.answers.part1 && out.part2 == A.answers.part2)
        {
            printf(COLOR_GREEN() "%s: OK [%5ld μs]\n" COLOR_RESET(), A.day_id.c_str(), int64_t(time));
        }
        else
        {
            printf(COLOR_RED() "%s: ERR [%5ld μs]\n" COLOR_RESET(), A.day_id.c_str(), int64_t(time));
            printf("  - Expected: pt1 = %zu, pt2 = %zu\n", A.answers.part1, A.answers.part2);
            printf("  - Actual  : pt1 = %zu, pt2 = %zu\n", out.part1, out.part2);
        }

        total_time += time;
    }

    printf("Total: %ld μs\n", int64_t(total_time));
    return 0;
}
