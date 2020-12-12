#include "advent2020.h"
#include <chrono>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

struct advent_t
{
    void (*fn)(const input_t&);
    std::string input_file;
};

static const advent_t advent2020[] = {
    // { day01, "input/day01.txt" },
    { day02, "input/day02.txt" },
    // { day03, "input/day03.txt" },
    // { day04, "input/day04.txt" },
    // { day05, "input/day05.txt" },
    // { day06, "input/day06.txt" },
    // { day07, "input/day07.txt" },
    // { day08, "input/day08.txt" },
    // { day09, "input/day09.txt" },
    // { day10, "input/day10.txt" },
    // { day11, "input/day11.txt" },
    // { day12, "input/day12.txt" },
    // { day13, "input/day13.txt" },
    // { day14, "input/day14.txt" },
    // { day15, "input/day15.txt" },
    // { day16, "input/day16.txt" },
    // { day17, "input/day17.txt" },
    // { day18, "input/day18.txt" },
    // { day19, "input/day19.txt" },
    // { day20, "input/day20.txt" },
    // { day21, "input/day21.txt" },
    // { day22, "input/day22.txt" },
    // { day23, "input/day23.txt" },
    // { day24, "input/day24.txt" },
    // { day25, "input/day25.txt" },
};

int main()
{
    double total_time = 0;

    for (auto& A : advent2020)
    {
        input_t input;
        load_input(input, A.input_file);

        auto t0 = std::chrono::steady_clock::now();
        (*A.fn)(input);
        auto elapsed = std::chrono::steady_clock::now() - t0;

        free_input(input);

        total_time += elapsed.count();
        printf(" [%ld μs]\n", int64_t(elapsed.count() * 1e-3));
    }
    printf("Total: %ld μs\n", int64_t(total_time * 1e-3));

    return 0;
}
