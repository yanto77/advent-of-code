#include "advent2020.h"
#include <fmt/format.h>

int main()
{
    int64_t total_time = 0;

    for (const auto& [year, days] : DayFactory::get_years())
    {
        for (const auto& [day, create_fn] : days)
        {
            const std::string& in_fname = fmt::format("input/{}/day{:02}.txt", year, day);
            const std::string& day_str = fmt::format("{}/{:02}", year, day);

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
