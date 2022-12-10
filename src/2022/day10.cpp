#include "setup/all.h"
#include <helpers/ocr.h>

ADVENT_DAY(2022, 10, 14040, 978538176);

namespace 
{
    constexpr bool PRINT_OCR_DEBUG = false;

    struct screen_t 
    {
        std::array<std::array<bool, 40>, 6> screen {};
        int64_t regX = 1;
        int64_t cycle = 0;

        int64_t part1_target = 20;
        int64_t part1_signal = 0;

        // `addx V`, two cycles, then X += V. (V can be negative.)
        void addx(int8_t value)
        {
            inc_cycle();
            inc_cycle();
            regX += value;
        }

        // `noop`, one cycle, no other effect.
        void noop()
        {
            inc_cycle();
        }

        void inc_cycle()
        {
            // Part 2 (drawing happens _during_ cycle)
            int64_t row = (cycle / 40);
            int64_t col = (cycle % 40);
            if (abs(col - regX) <= 1)
            {
                screen[row][col] = true;
            }

            cycle++; 

            // Part 1 (signal is checked _after_ cycle)
            if ((part1_target - cycle) <= 0)
            {
                part1_signal += (part1_target * regX);
                part1_target += 40;
            }
        }

        size_t get_letter_score()
        {
            std::array<char, 8> result {};

            // expect 8 chars, each 6x4 with 6x1 spacing in between.
            constexpr size_t CHAR_N = 8;
            constexpr size_t CHAR_Y = 6;
            constexpr size_t CHAR_X = 4;
            for (int letterIdx = 0; letterIdx < CHAR_N; letterIdx++) 
            {
                std::array<char, CHAR_X*CHAR_Y> span;
                for (size_t y = 0; y < screen.size(); y++)
                {
                    for (size_t x = 0; x < CHAR_X; x++)
                    {
                        bool is_set = screen[y][letterIdx*(CHAR_X+1) + x];
                        span[y*CHAR_X + x] = (is_set) ? '#' : '.';
                    }
                }

                result[letterIdx] = get_letter(span);
            }
           
            if constexpr (PRINT_OCR_DEBUG)
            {
                fmt::print("Screen:\n");
                for (auto& row: screen)
                {
                    for (bool cell: row)
                    {
                        if (cell) fmt::print(COLOR_GREEN() "#");
                        else fmt::print(COLOR_GRAY() ".");
                    }
                    fmt::print("\n");
                }
                fmt::print(COLOR_RESET());
                fmt::print("Parsed: {}\n", fmt::join(result, ""));
            }

            return get_str_score(result); // ZGCJZJFL
        }
    };
}

result_t Day_2022_10::run_solution(str_view input) const
{
    screen_t screen;
    for_each_split(input, '\n', [&](str_view line)
    {
        if (line[0] == 'n')
        {
            screen.noop();
        }
        else if (line[0] == 'a')
        {
            int8_t value = to_int<int8_t>(line.substr(5));
            screen.addx(value);
        }
    });

    size_t part1 = screen.part1_signal;
    size_t part2 = screen.get_letter_score(); 
    return { part1, part2 };
}

void Day_2022_10::run_tests() const
{
}
