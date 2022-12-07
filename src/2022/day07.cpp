#include "setup/all.h"
#include <unordered_map>

ADVENT_DAY(2022, 7, 1427048, 2940614);

namespace 
{
    using filesystem_t = std::unordered_map<std::string, size_t>;

    filesystem_t parse_logs(str_view input)
    {
        filesystem_t fs;
        std::vector<str_view> cwd; // TODO: stack

        for_each_split(input, '\n', [&](str_view line)
        {
            if (line.starts_with("$ cd"))
            {
                str_view arg = line.substr(5);
                if (arg == "..") 
                    cwd.pop_back();
                else
                    cwd.push_back(arg);

                std::string fullpath;
                for (auto part: cwd)
                {
                    fullpath.append("/");
                    fullpath.append(part);
                }

                if (!fs.contains(fullpath))
                {
                    fs.insert({ fullpath, 0});
                }
            }
            else if (line.starts_with("$ ls") || line.starts_with("dir "))
            {
                // ignore
            }
            else // it's a file
            {
                uint32_t file_size = 0;
                parse_uint_scalar(line, [&](uint32_t num) { file_size = num; });
                
                std::string parentpath;
                for (auto part: cwd)
                {
                    parentpath.append("/");
                    parentpath.append(part);

                    size_t& parent_size = fs.find(parentpath)->second;
                    parent_size += file_size;
                }
            }
        });

        return fs;
    }
}

result_t Day_2022_7::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = SIZE_MAX;

    // total dir number is 159
    filesystem_t fs = parse_logs(input);

    size_t free = 70000000 - (fs.find("//")->second);
    size_t need = 30000000;
    size_t need_more = need - free;
    for (auto [name, size]: fs)
    {
        if (size <= 100000)
        {
            part1 += size;
        }
        
        if (size > need_more)
        {
            part2 = min(part2, size);
        }
    }

    return { part1, part2 };
}

void Day_2022_7::run_tests() const
{
}
