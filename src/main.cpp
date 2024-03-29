#include "setup/all.h"
#include <chrono>
#include <fstream>
#include <filesystem>

namespace
{
    static const char* OK = COLOR_GREEN() "OK" COLOR_RESET();
    static const char* ERR = COLOR_RED() "ERR" COLOR_RESET();

    // Ref: https://blog.insane.engineer/post/cpp_read_file_into_string/
    inline std::string load_input(str_view filename)
    {
        // Sanity check
        if (!std::filesystem::is_regular_file(filename))
            return { };

        // Open the file
        // Note that we have to use binary mode as we want to return a string
        // representing matching the bytes of the file on the file system.
        std::ifstream file(filename.data(), std::ios::in | std::ios::binary);
        if (!file.is_open())
            return { };

        // Read contents
        std::string content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

        // Close the file
        file.close();

        return content;
    }

    int64_t run_single_day(size_t year, size_t day, DayFactory::create_fn_t create_fn)
    {
        // Run static tests (if any)
        const std::unique_ptr<IDay> inst = create_fn();
        inst->run_tests();

        // Load input file once
        const std::string in_fname = fmt::format("input/{}/day{:02}.txt", year, day);
        const std::string input = load_input(in_fname);

        // Run the solution multiple times to collect timings
        std::vector<int64_t> timings;
        for (int i = 0; i < 10; i++)
        {
            auto t0 = std::chrono::steady_clock::now();
            result_t actual = inst->run_solution(input);
            auto elapsed = (std::chrono::steady_clock::now() - t0);
            int64_t time = (elapsed.count() / 1000);
            timings.push_back(time);

            if (i == 0) // Validate only once
            {
                result_t expected = inst->get_valid();
                if (expected != actual)
                {
                    fmt::print("{}/{:02}: {} [{:6} μs]\n", year, day, ERR, time);
                    fmt::print("  - Expected: pt1 = {}  pt2 = {}\n", expected.part1, expected.part2);
                    fmt::print("  - Actual  : pt1 = {}  pt2 = {}\n", actual.part1, actual.part2);
                    return 0;
                }
            }
        }

        int64_t min_time = *(std::min_element(timings.begin(), timings.end()));
        fmt::print("{}/{:02}: {} [{:6} μs]\n", year, day, OK, min_time);
        return min_time;
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
