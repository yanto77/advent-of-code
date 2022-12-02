#include "setup/all.h"

namespace
{
    static const char* OK = COLOR_GREEN() "OK" COLOR_RESET();

    int64_t run_single_day(size_t year, size_t day, DayFactory::create_fn_t create_fn)
    {
        // Run static tests (if any)
        const std::unique_ptr<IDay> inst = create_fn();
        inst->run_tests();

        // Load input file once
        const std::string in_fname = fmt::format("input/{}/day{:02}.txt", year, day);
        const std::string input = load_input(in_fname);

        // Run once to validate result
        output_t actual = inst->run_solution(input);
        output_t expected = inst->get_valid();
        if (expected != actual)
        {
            fmt::print("{}/{:02}: ERR\n", year, day);
            fmt::print("  - Expected: pt1 = {}  pt2 = {}\n", expected.part1, expected.part2);
            fmt::print("  - Actual  : pt1 = {}  pt2 = {}\n", actual.part1, actual.part2);
            return 0;
        }

        // Run more times to collect timings
        std::vector<int64_t> timings;
        for (int i = 0; i < 10; i++)
        {
            auto t0 = std::chrono::steady_clock::now();
            inst->run_solution(input);
            auto elapsed = (std::chrono::steady_clock::now() - t0);
            int64_t time = (elapsed.count() / 1000);
            timings.push_back(time);
        }

        auto [min, max] = std::minmax_element(timings.begin(), timings.end());
        fmt::print("{}/{:02}: {} [min {:6} μs, max {:6} μs]\n", year, day, OK, *min, *max);
        return *min;
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

            const int64_t min_time = run_single_day(year, day, create_fn);

            time_year += min_time;
            time_total += min_time;
        }

        fmt::print("=> Total for year {}: {:6} μs\n\n", year, time_year);
    }

    fmt::print("=> Total for all years: {:6} μs\n\n", time_total);
    return 0;
}
