#include "advent2020.h"
#include "setup/days.h"
// #include <format>
#include <fmt/format.h>

struct advent_t
{
    std::string day_id;
    std::string input_file;

    output_t (*fn_exec)(const input_t&);
    void (*fn_test)();

    output_t answers;
};

static std::vector<advent_t> g_advent2020 = {
    advent_t { "Day 02", "input/2020/day02.txt", day02, day02_test, output_t { 465, 294 } },
    advent_t { "Day 03", "input/2020/day03.txt", day03, day03_test, output_t { 268, 3093068400 } },
    advent_t { "Day 04", "input/2020/day04.txt", day04, day04_test, output_t { 237, 172 } },
    advent_t { "Day 05", "input/2020/day05.txt", day05, day05_test, output_t { 953, 615 } },
    advent_t { "Day 06", "input/2020/day06.txt", day06, day06_test, output_t { 6947, 3398 } },
    advent_t { "Day 07", "input/2020/day07.txt", day07, day07_test, output_t { 172, 39645 } },
    advent_t { "Day 08", "input/2020/day08.txt", day08, day08_test, output_t { 1949, 2092 } },
    advent_t { "Day 09", "input/2020/day09.txt", day09, day09_test, output_t { 507622668, 76688505 } },
    advent_t { "Day 10", "input/2020/day10.txt", day10, day10_test, output_t { 2664, 148098383347712 } },
    advent_t { "Day 11", "input/2020/day11.txt", day11, day11_test, output_t { 2108, 1897 } },
    advent_t { "Day 12", "input/2020/day12.txt", day12, day12_test, output_t { 364, 39518 } },
    advent_t { "Day 13", "input/2020/day13.txt", day13, day13_test, output_t { 2947, 526090562196173 } },
    advent_t { "Day 14", "input/2020/day14.txt", day14, day14_test, output_t { 14954914379452, 3415488160714 } },
    advent_t { "Day 15", "input/2020/day15.txt", day15, day15_test, output_t { 1294, 573522 } },
    advent_t { "Day 16", "input/2020/day16.txt", day16, day16_test, output_t { 21956, 3709435214239 } },
    advent_t { "Day 17", "input/2020/day17.txt", day17, day17_test, output_t { 401, 2224 } },
    advent_t { "Day 18", "input/2020/day18.txt", day18, day18_test, output_t { 4696493914530, 362880372308125 } },
    advent_t { "Day 19", "input/2020/day19.txt", day19, day19_test, output_t { 208, 316 } },
    advent_t { "Day 20", "input/2020/day20.txt", day20, day20_test, output_t { 107399567124539, 1555 } },
    advent_t { "Day 21", "input/2020/day21.txt", day21, day21_test, output_t { 2307, 1 } },
    advent_t { "Day 22", "input/2020/day22.txt", day22, day22_test, output_t { 33434, 31657 } },
    advent_t { "Day 23", "input/2020/day23.txt", day23, day23_test, output_t { 53248976, 418819514477 } },
    advent_t { "Day 24", "input/2020/day24.txt", day24, day24_test, output_t { 438, 4038 } },
    advent_t { "Day 25", "input/2020/day25.txt", day25, day25_test, output_t { 6408263, 1 } },
};

int main()
{
    int64_t total_time = 0;

    for (const auto& A : g_advent2020)
    {
        // Run some initial tests
        A.fn_test();

        // Run the actual task with actual input
        input_t input = load_input(A.input_file);
        auto t0 = std::chrono::steady_clock::now();
        output_t out = (*A.fn_exec)(input);
        auto elapsed = (std::chrono::steady_clock::now() - t0);
        int64_t time = (elapsed.count() / 1000);
        free_input(input);

        // Output
        if (out.part1 == 0 || out.part2 == 0)
        {
            printf(COLOR_YELLOW() "%s: TODO [%6ld μs]\n" COLOR_RESET(), A.day_id.c_str(), time);
            printf("  - Expected: pt1 = %zu, pt2 = %zu\n", A.answers.part1, A.answers.part2);
            printf("  - Actual  : pt1 = %zu, pt2 = %zu\n", out.part1, out.part2);
        }
        else if (out.part1 == A.answers.part1 && out.part2 == A.answers.part2)
        {
            printf(COLOR_GREEN() "%s: OK [%6ld μs]\n" COLOR_RESET(), A.day_id.c_str(), time);
        }
        else
        {
            printf(COLOR_RED() "%s: ERR [%6ld μs]\n" COLOR_RESET(), A.day_id.c_str(), time);
            printf("  - Expected: pt1 = %zu, pt2 = %zu\n", A.answers.part1, A.answers.part2);
            printf("  - Actual  : pt1 = %zu, pt2 = %zu\n", out.part1, out.part2);
        }

        total_time += time;
    }

    for (const auto& [year, days] : DayFactory::get_years())
    {
        for (const auto& [day, create_fn] : days)
        {
            const std::string& in_fname = fmt::format("input/{}/day{:02}.txt", year, day);
            const std::string& day_str = fmt::format("{:02}", day);

            const std::unique_ptr<IDay> inst = create_fn();
            inst->run_tests();

            // Run the actual task with actual input
            input_t input = load_input(in_fname);
            auto t0 = std::chrono::steady_clock::now();
            output_t out = inst->run_solution(input);
            auto elapsed = (std::chrono::steady_clock::now() - t0);
            int64_t time = (elapsed.count() / 1000);
            free_input(input);

            auto valid = inst->get_valid();
            if (out.part1 == 0 || out.part2 == 0)
            {
                printf(COLOR_YELLOW() "%s: TODO [%6ld μs]\n" COLOR_RESET(), day_str.c_str(), time);
                printf("  - Expected: pt1 = %zu, pt2 = %zu\n", valid.part1, valid.part2);
                printf("  - Actual  : pt1 = %zu, pt2 = %zu\n", out.part1, out.part2);
            }
            else if (out.part1 == valid.part1 && out.part2 == valid.part2)
            {
                printf(COLOR_GREEN() "%s: OK [%6ld μs]\n" COLOR_RESET(), day_str.c_str(), time);
            }
            else
            {
                printf(COLOR_RED() "%s: ERR [%6ld μs]\n" COLOR_RESET(), day_str.c_str(), time);
                printf("  - Expected: pt1 = %zu, pt2 = %zu\n", valid.part1, valid.part2);
                printf("  - Actual  : pt1 = %zu, pt2 = %zu\n", out.part1, out.part2);
            }

            total_time += time;
        }
    }

    printf("Total: %ld μs\n", total_time);
    return 0;
}
