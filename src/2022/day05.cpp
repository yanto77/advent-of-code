#include "setup/all.h"

ADVENT_DAY(2022, 5, 9280497752, 7659589992);

namespace 
{
    using row_t = std::array<uint8_t, 60>;
    using map_t = std::array<row_t, 9>;
    using move_t = std::array<uint8_t, 3>; // { count, from, to }
    using moves_t = std::vector<move_t>;
    struct data_t { map_t map; moves_t moves; };

    data_t parse_map(str_view input)
    {
        map_t map {};
        moves_t moves {};

        const size_t idx = input.find("move");
        const str_view mapStr { &input[0], idx };
        const str_view moveStr { &input[idx], input.size() - idx };

        // assumes well-formed input
        const size_t LINE_SIZE = 36;
        const size_t LINE_COUNT = (mapStr.size() / LINE_SIZE) - 1;
        for (size_t y = 0; y < mapStr.size() / LINE_SIZE; y++)
        {
            const char* line = &mapStr[y * LINE_SIZE];

            if (line[0] != '[') 
            { 
                break; 
            }

            for (size_t x = 1; x < LINE_SIZE; x += 4)
            {
                if (line[x] != ' ')
                {
                    map[(x - 1) / 4][(LINE_COUNT - y - 1)] = line[x];
                }
            }
        }

        for_each_split(moveStr, '\n', [&](str_view line)
        {
            size_t moveIdx = 0;
            move_t move {};
            parse_uint_scalar(line, [&](uint8_t num)
            {
                move[moveIdx++] = num;
            });

            move[1]--; move[2]--; // our array is 0-index, input is 1-index
            moves.push_back(move);
        });
        
        return {map, moves};
    }

    ssize_t find_top(const row_t& row)
    {
        for (size_t i = 0; i < row.size(); i++)
            if (row[i] == 0)
                return (i - 1);

        assert(false && "stack is full, not enough room.");
        return 0;
    }

    size_t get_score(const map_t& map)
    {
        size_t out = 0;
        for (auto row: map)
        {
            auto value = row[find_top(row)];
            out = 10 * out + value;
        }

        constexpr bool PRINT = false;
        if constexpr (PRINT)
        {
            fmt::print("score: ");
            for (auto row: map)
            {
                fmt::print("{}", (char)row[find_top(row)]);
            }
            fmt::print("\n");
        }

        return out;
    }
}

result_t Day_2022_5::run_solution(str_view input) const
{
    
    auto [map1, moves] = parse_map(input);
    map_t map2 = map1;

    for (auto [count, from, to]: moves)
    {
        // we're moving the same number of crates, so these values work for both maps
        const ssize_t fromTop = find_top(map1[from]);
        const ssize_t toTop = find_top(map1[to]);

        for (size_t i = 0; i < count; i++)
        {
            std::swap(map1[to][toTop+1+i], map1[from][fromTop-i]);
            std::swap(map2[to][toTop+1+i], map2[from][fromTop-count+1+i]);
        }
    }

    size_t part1 = get_score(map1); // TPGVQPFDH
    size_t part2 = get_score(map2); // DMRDFRHHH
    return { part1, part2 };
}

void Day_2022_5::run_tests() const
{
}
