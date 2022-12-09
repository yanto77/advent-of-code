#include "setup/all.h"
#include <unordered_map>

ADVENT_DAY(2022, 7, 1427048, 2940614);

namespace 
{
    // returns vector of sizes, where each index corresponds to each unique dir.
    // (e.g., dirs[0] is "/", dirs[1] is "/abc", dirs[2] is "/abc/def", etc.)
    std::vector<size_t> parse_logs(str_view input)
    {
        std::vector<size_t> cwd; // stack of indexes into `dirs`
        std::vector<size_t> dirs;
        dirs.reserve(256); // input has 159 dirs

        for_each_split(input, '\n', [&](str_view line)
        {
            if (line.starts_with("$ cd"))
            {
                if (line.substr(5) == "..") 
                {
                    cwd.pop_back();
                }
                else 
                {
                    dirs.push_back(0); // new dir entry
                    cwd.push_back(dirs.size() - 1); // new dir index
                }
            }
            else if (!line.starts_with("$ ls") && !line.starts_with("dir ")) // it's a file
            {
                const uint32_t file_size = to_int<uint32_t>(line);
                for (size_t index: cwd) // file's size is added to each parent
                {
                    dirs[index] += file_size;
                }
            }
        });

        return dirs;
    }
}

result_t Day_2022_7::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = SIZE_MAX;

    std::vector<size_t> dirs = parse_logs(input);

    const size_t need_more = dirs[0] - 40000000; // dirs[0] is the size of '/'
    for (auto size: dirs)
    {
        if (size <= 100000) part1 += size;
        if (size > need_more) part2 = min(part2, size);
    }

    return { part1, part2 };
}

void Day_2022_7::run_tests() const
{
}
