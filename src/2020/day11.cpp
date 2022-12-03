#include "setup/all.h"

ADVENT_DAY(2020, 11, 2108, 1897);

enum class seat_state_t : uint8_t
{
    FLOOR,
    TAKEN,
    EMPTY
};
typedef std::vector<std::vector<seat_state_t>> seat_map_t;

constexpr bool DEBUG_VISUALIZE = false;

namespace
{
    [[maybe_unused]]
    void print_seat(const seat_state_t seat)
    {
        if constexpr (!DEBUG_VISUALIZE)
            return;

        switch (seat)
        {
            case seat_state_t::FLOOR: { printf(COLOR_GRAY()  "."); break; }
            case seat_state_t::TAKEN: { printf(COLOR_RED()   "#"); break; }
            case seat_state_t::EMPTY: { printf(COLOR_GREEN() "L"); break; }
        }

        printf(COLOR_RESET());
    }

    [[maybe_unused]]
    void print_seat_map(const seat_map_t& map)
    {
        if constexpr (!DEBUG_VISUALIZE)
            return;

        for (const auto& row : map)
        {
            for (const seat_state_t seat : row)
                print_seat(seat);

            printf("\n");
        }
    }

    seat_map_t parse(str_view input)
    {
        seat_map_t seats;
        ssize_t len = -1;

        for_each_split(input, '\n', [&](str_view line_in) {
            if (len == -1)
                len = line_in.size();

            std::vector<seat_state_t> line_out;
            line_out.reserve(len);

            for (const char ch : line_in)
            {
                if (ch == 'L')
                    line_out.push_back(seat_state_t::EMPTY);
                else if (ch == '.')
                    line_out.push_back(seat_state_t::FLOOR);
                else if (ch == '#')
                    line_out.push_back(seat_state_t::TAKEN);
            }

            seats.push_back(line_out);
        });

        return seats;
    }

    seat_map_t pad(const seat_map_t& input, const vec2i& padding)
    {
        const size_t in_rows = input.size();
        const size_t in_cols = input[0].size();

        const size_t rows = in_rows + padding.y * 2;
        const size_t cols = in_cols + padding.x * 2;
        seat_map_t padded(rows, std::vector<seat_state_t>(cols, seat_state_t::FLOOR));

        for (size_t y = 0; y < in_rows; ++y)
        {
            for (size_t x = 0; x < in_cols; ++x)
            {
                padded[y + padding.y][x + padding.x] = input[y][x];
            }
        }

        return padded;
    }

    seat_map_t unpad(const seat_map_t& input, const vec2i& padding)
    {
        const size_t rows = input.size() - padding.x * 2;
        const size_t cols = input[0].size() - padding.y * 2;
        seat_map_t unpadded(rows, std::vector<seat_state_t>(cols, seat_state_t::FLOOR));

        for (size_t y = 0; y < rows; ++y)
        {
            for (size_t x = 0; x < cols; ++x)
            {
                unpadded[y][x] = input[y + padding.y][x + padding.x];
            }
        }

        return unpadded;
    }

    size_t get_occupied_seats_pt1(const seat_map_t& input, size_t cell_x, size_t cell_y)
    {
        size_t occupied_nb = 0;
        for (int wy = -1; wy <= 1; ++wy)
        {
            for (int wx = -1; wx <= 1; ++wx)
            {
                if (wy == 0 && wx == 0)
                    continue; // skip center

                if (input[cell_y + wy][cell_x + wx] == seat_state_t::TAKEN)
                    ++occupied_nb;
            }
        }
        return occupied_nb;
    }

    size_t get_occupied_seats_pt2(const seat_map_t& input, size_t cell_x, size_t cell_y)
    {
        size_t occupied_nb = 0;

        const std::array<vec2i, 8> directions =
        {
            vec2i{-1,  1}, vec2i{0,  1}, vec2i{1,  1},
            vec2i{-1,  0},               vec2i{1,  0},
            vec2i{-1, -1}, vec2i{0, -1}, vec2i{1, -1},
        };

        const size_t height = input.size();
        const size_t width = input[0].size();
        for (const auto& dir : directions)
        {
            vec2i pos = { static_cast<int>(cell_x), static_cast<int>(cell_y) };
            pos += dir;

            while (0 <= pos.x && pos.x < width && 0 <= pos.y && pos.y < height)
            {
                if (input[pos.y][pos.x] == seat_state_t::TAKEN)
                {
                    ++occupied_nb;
                    break;
                }

                if (input[pos.y][pos.x] == seat_state_t::EMPTY)
                {
                    break;
                }

                pos += dir;
            }
        }

        return occupied_nb;
    }

    std::pair<size_t, seat_map_t> simulate_until_stable(const seat_map_t& input, bool part1)
    {
        size_t steps = 0;

        constexpr vec2i padding = { 1, 1 };

        seat_map_t current = pad(input, padding);
        const size_t height = input.size();
        const size_t width = input[0].size();

        for (;;)
        {
            seat_map_t next = current;

            // For the whole map
            for (size_t y = padding.y; y < height + padding.y; ++y)
            {
                for (size_t x = padding.x; x < width + padding.x; ++x)
                {
                    // Find occupied seats from the closest 3x3 window
                    size_t occupied_nb = part1 ?
                        get_occupied_seats_pt1(current, x, y) :
                        get_occupied_seats_pt2(current, x, y);

                    // Update state
                    if (current[y][x] == seat_state_t::EMPTY && occupied_nb == 0)
                    {
                        next[y][x] = seat_state_t::TAKEN;
                    }
                    else if (part1 && current[y][x] == seat_state_t::TAKEN && occupied_nb >= 4)
                    {
                        next[y][x] = seat_state_t::EMPTY;
                    }
                    else if (!part1 && current[y][x] == seat_state_t::TAKEN && occupied_nb >= 5)
                    {
                        next[y][x] = seat_state_t::EMPTY;
                    }
                }
            }

            if constexpr (DEBUG_VISUALIZE)
            {
                clear_screen();
                printf("\n\nStep: %zu\n", steps + 1);
                print_seat_map(next);
                getchar();
            }

            if (next != current)
            {
                current = next;
                ++steps;
            }
            else
                break;
        }

        return { steps, unpad(current, padding) };
    }

    size_t count_occupied(const seat_map_t& input)
    {
        size_t occupied = 0;

        for (const auto& row : input)
            for (const seat_state_t seat : row)
                if (seat == seat_state_t::TAKEN)
                    ++occupied;

        return occupied;
    }
}

result_t Day_2020_11::run_solution(str_view input) const
{
    seat_map_t test_map = parse(input);
    const auto& [steps, after_map] = simulate_until_stable(test_map, true);
    size_t occupied1 = count_occupied(after_map);

    const auto& [steps2, after_map2] = simulate_until_stable(test_map, false);
    size_t occupied2 = count_occupied(after_map2);

    return { occupied1, occupied2 };
}

void Day_2020_11::run_tests() const
{
    char str1[] = "L.LL.LL.LL\n"
                  "LLLLLLL.LL\n"
                  "L.L.L..L..\n"
                  "LLLL.LL.LL\n"
                  "L.LL.LL.LL\n"
                  "L.LLLLL.LL\n"
                  "..L.L.....\n"
                  "LLLLLLLLLL\n"
                  "L.LLLLLL.L\n"
                  "L.LLLLL.LL\n";

    {
        seat_map_t test_map = parse(str1);
        const auto& [steps, after_map] = simulate_until_stable(test_map, true);

        char str2[] = "#.#L.L#.##\n"
                      "#LLL#LL.L#\n"
                      "L.#.L..#..\n"
                      "#L##.##.L#\n"
                      "#.#L.LL.LL\n"
                      "#.#L#L#.##\n"
                      "..L.L.....\n"
                      "#L#L##L#L#\n"
                      "#.LLLLLL.L\n"
                      "#.#L#L#.##\n";
        seat_map_t expected_map = parse(str2);
        assert(expected_map == after_map);
        assert(steps == 5);
        assert(count_occupied(after_map) == 37);
    }
    {
        seat_map_t test_map = parse(str1);
        const auto& [steps, after_map] = simulate_until_stable(test_map, false);

        char str2[] = "#.L#.L#.L#\n"
                      "#LLLLLL.LL\n"
                      "L.L.L..#..\n"
                      "##L#.#L.L#\n"
                      "L.L#.LL.L#\n"
                      "#.LLLL#.LL\n"
                      "..#.L.....\n"
                      "LLL###LLL#\n"
                      "#.LLLLL#.L\n"
                      "#.L#LL#.L#\n";
        seat_map_t expected_map = parse(str2);
        assert(expected_map == after_map);
        assert(steps == 6);
        assert(count_occupied(after_map) == 26);
    }
}
