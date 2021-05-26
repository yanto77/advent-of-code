#include "setup/all.h"
#include <fmt/format.h>

namespace
{
    enum class valid_t { TODO, OK, ERR };

    valid_t validate(const output_t& expected, const output_t& actual)
    {
        if (actual.part1 == 0 || actual.part2 == 0)
            return valid_t::TODO;
        else if (actual.part1 == expected.part1 && actual.part2 == expected.part2)
            return valid_t::OK;
        else
            return valid_t::ERR;
    }

    std::string to_str(valid_t value)
    {
        if (value == valid_t::TODO)
            return { COLOR_YELLOW() "TODO" COLOR_RESET() };
        else if (value == valid_t::OK)
            return { COLOR_GREEN() "OK" COLOR_RESET() };
        else
            return { COLOR_RED() "ERR" COLOR_RESET() };
    }

    std::pair<output_t, int64_t> run_single_day(size_t year, size_t day, IDay* inst)
    {
        inst->run_tests();

        const std::string& in_fname = fmt::format("input/{}/day{:02}.txt", year, day);
        input_t input = load_input(in_fname);

        auto t0 = std::chrono::steady_clock::now();
        output_t result = inst->run_solution(input);
        auto elapsed = (std::chrono::steady_clock::now() - t0);
        int64_t time = (elapsed.count() / 1000);

        free_input(input);

        return { result, time };
    }

    void print_results(size_t year, size_t day, const output_t& expected, const output_t& actual, int64_t time)
    {
        const std::string& day_str = fmt::format("{}/{:02}", year, day);
        valid_t valid = validate(expected, actual);
        fmt::print("{}: {} [{:6} μs]\n", day_str, to_str(valid), time);
        if (valid != valid_t::OK)
        {
            fmt::print("  - Expected: pt1 = {}, pt2 = {}\n", expected.part1, expected.part2);
            fmt::print("  - Actual  : pt1 = {}, pt2 = {}\n", actual.part1, actual.part2);
        }
    }
}

int main(int argc, char *argv[])
{
    size_t filter_year = (argc > 1) ? std::atoi(argv[1]) : 0;
    size_t filter_day = (argc > 2) ? std::atoi(argv[2]) : 0;

    int64_t time_total = 0;
    for (const auto& [year, days] : DayFactory::get_years())
    {
        if (filter_year != 0 && year != filter_year)
            continue;

        int64_t time_year = 0;
        for (const auto& [day, create_fn] : days)
        {
            if (filter_day != 0 && day != filter_day)
                continue;

            const std::unique_ptr<IDay> inst = create_fn();
            const auto& [result, time] = run_single_day(year, day, inst.get());
            print_results(year, day, inst->get_valid(), result, time);

            time_year += time;
            time_total += time;
        }

        fmt::print("=> Total for year {}: {:6} μs\n\n", year, time_year);
    }

    fmt::print("=> Total for all years: {:6} μs\n\n", time_total);
    return 0;
}
