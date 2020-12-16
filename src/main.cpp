#include "advent2020.h"
#include <chrono>

struct advent_t
{
    std::string day_id;
    void (*fn)(const input_t&);
    std::string input_file;
};

static const advent_t advent2020[] = {
    // { "Day 01", day01, "input/day01.txt" },
    // { "Day 02", day02, "input/day02.txt" },
    // { "Day 03", day03, "input/day03.txt" },
    // { "Day 04", day04, "input/day04.txt" },
    { "Day 05", day05, "input/day05.txt" },
    // { "Day 06", day06, "input/day06.txt" },
    // { "Day 07", day07, "input/day07.txt" },
    // { "Day 08", day08, "input/day08.txt" },
    // { "Day 09", day09, "input/day09.txt" },
    // { "Day 10", day10, "input/day10.txt" },
    // { "Day 11", day11, "input/day11.txt" },
    // { "Day 12", day12, "input/day12.txt" },
    // { "Day 13", day13, "input/day13.txt" },
    // { "Day 14", day14, "input/day14.txt" },
    // { "Day 15", day15, "input/day15.txt" },
    // { "Day 16", day16, "input/day16.txt" },
    // { "Day 17", day17, "input/day17.txt" },
    // { "Day 18", day18, "input/day18.txt" },
    // { "Day 19", day19, "input/day19.txt" },
    // { "Day 20", day20, "input/day20.txt" },
    // { "Day 21", day21, "input/day21.txt" },
    // { "Day 22", day22, "input/day22.txt" },
    // { "Day 23", day23, "input/day23.txt" },
    // { "Day 24", day24, "input/day24.txt" },
    // { "Day 25", day25, "input/day25.txt" },
};

int main()
{
    double total_time = 0;

    for (auto& A : advent2020)
    {
        printf("%s: ", A.day_id.c_str());

        input_t input;
        load_input(input, A.input_file);

        auto t0 = std::chrono::steady_clock::now();
        (*A.fn)(input);
        auto elapsed = std::chrono::steady_clock::now() - t0;

        free_input(input);

        total_time += elapsed.count();
        #ifdef __linux__
        constexpr const char* msg = " [%ld μs]\n";
        #elif _WIN32
        constexpr const char* msg = " [%ld us]\n";
        #endif
        printf(msg, int64_t(elapsed.count() * 1e-3));
    }

    #ifdef __linux__
    constexpr const char* msg = "Total: %ld μs\n";
    #elif _WIN32
    constexpr const char* msg = "Total: %ld us\n";
    #endif
    printf(msg, int64_t(total_time * 1e-3));

    return 0;
}
