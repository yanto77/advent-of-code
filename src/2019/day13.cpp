#include "setup/all.h"
#include "intcode.h"

#include <unistd.h>
#include <termios.h>

ADVENT_DAY(2019, 13, 306, 15328);

namespace
{
    enum { T_EMPTY, T_WALL, T_BLOCK, T_PADDLE, T_BALL };

    struct game_t 
    {
        intcode_solver_t solver;
        
        static const size_t X = 50;
        static const size_t Y = 30;
        std::array<std::array<uint8_t, X>, Y> tiles;

        game_t(const input_t& input): solver(get_sv(input)), tiles{}
        {
        }

        [[maybe_unused]]
        void print_map() const
        {
            printf("\x1B[2J\x1B[H"); // clear screen and move cursor to top-left corner
            printf("map: \n");

            for (size_t y = 0; y < Y; y++)
            {
                for (size_t x = 0; x < X; x++)
                {
                    switch (tiles[y][x])
                    {
                        case T_WALL: { printf(COLOR_GRAY() "W" COLOR_RESET()); break; }
                        case T_BLOCK: { printf(COLOR_WHITE() "B" COLOR_RESET()); break; }
                        case T_PADDLE: { printf(COLOR_YELLOW() "_" COLOR_RESET()); break; }
                        case T_BALL: { printf(COLOR_GREEN() "o" COLOR_RESET()); break; }
                        case T_EMPTY: { printf(" "); break; }
                    }
                }
                printf("\n");
            }
        }
    };
}

output_t Day_2019_13::run_solution(const input_t& input) const
{
    size_t part1 = 0, part2 = 0;

    // Part 1
    {
        game_t game(input);
        while (!game.solver.is_halted)
        {
            game.solver.execute(false);
        }

        const auto& output = game.solver.output_data;
        for (size_t i = 0; i < output.size(); i += 3)
        {
            part1 += (static_cast<uint8_t>(output[i + 2]) == T_BLOCK);
        }
    }

    // Part 2
    {
        game_t game(input);
        game.tiles = {};
        game.solver.memory[0] = 2; // set free-to-play mode

        size_t ball = 0, paddle = 0, score = 0;
        while (!game.solver.is_halted)
        {
            uint8_t op = game.solver.execute(true);
            if (game.solver.is_halted)
            {
                break;
            }
            else if (op == instr::INPUT)
            {
                auto joystick = (paddle > ball) ? -1 : (paddle < ball) ? 1 : 0;
                *game.solver.idp = joystick;

                // game.print_map();
                // std::this_thread::sleep_for(1ms);
            }
            else
            {
                game.solver.execute(true);
                game.solver.execute(true);

                const auto& out = game.solver.output_data;
                const auto x = out[0];
                const auto y = out[1];
                const auto z = out[2];

                if (x == -1 && y == 0)
                {
                    score = z;
                }
                else // update tile
                {
                    game.tiles[y][x] = static_cast<uint8_t>(z);

                    if (z == T_PADDLE)
                        paddle = x;
                    else if (z == T_BALL)
                        ball = x;
                }
                
                game.solver.output_data.clear();
            }
        }

        part2 = score;
    }

    return {part1, part2};
}

void Day_2019_13::run_tests() const
{
}
