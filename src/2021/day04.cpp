#include "setup/all.h"

ADVENT_DAY(2021, 4, 49686, 26878);

namespace
{
    struct card_t
    {
        std::array<std::array<int, 5>, 5> numbers {};
        std::bitset<5*5> marks = 0;

        void mark_number(uint8_t number)
        {
            for (int row = 0; row < 5; row++)
                for (int col = 0; col < 5; col++)
                    if (numbers[row][col] == number)
                        marks.set(row * 5 + col);
        }

        bool has_win() const
        {
            static const std::bitset<5*5> valid_set[] = {
                // horizontal
                0b11111'00000'00000'00000'00000,
                0b00000'11111'00000'00000'00000,
                0b00000'00000'11111'00000'00000,
                0b00000'00000'00000'11111'00000,
                0b00000'00000'00000'00000'11111,
                // vertical
                0b10000'10000'10000'10000'10000,
                0b01000'01000'01000'01000'01000,
                0b00100'00100'00100'00100'00100,
                0b00010'00010'00010'00010'00010,
                0b00001'00001'00001'00001'00001,
            };

            for (auto valid_mark: valid_set)
                if ((marks & valid_mark) == valid_mark)
                    return true;

            return false;
        }

        size_t get_unmarked_score() const
        {
            size_t score = 0;

            for (int row = 0; row < 5; row++)
                for (int col = 0; col < 5; col++)
                    if (!marks.test(row * 5 + col))
                        score += numbers[row][col];

            return score;
        }

        [[maybe_unused]]
        void debug_print() const
        {
            for (int row = 0; row < 5; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    if (marks.test(row * 5 + col))
                    {
                        fmt::print(COLOR_GREEN() "{:2d} " COLOR_RESET(), numbers[row][col]);
                    }
                    else
                    {
                        fmt::print(COLOR_GRAY() "{:2d} " COLOR_RESET(), numbers[row][col]);
                    }
                }
                printf("\n");
            }
            fmt::print("marks: {:#b}\n", marks.to_ullong());
            fmt::print("score: {}\n", get_unmarked_score());
            printf("\n");
        }
    };

    struct game_t
    {
        std::vector<uint8_t> numbers {};
        std::vector<card_t> cards {};
    };

    game_t parse(str_view input)
    {
        bool parse_cards = false;
        bool skip_next = false;
        game_t game_data;

        parse_input(input, [&](str_view line)
        {
            if (skip_next)
            {
                skip_next = false;
                return;
            }

            if (!parse_cards)
            {
                game_data.numbers = to_multi_int<uint8_t>(line);
                parse_cards = true;
                skip_next = true;
            }
            else
            {
                static card_t temp_card {};
                static uint8_t card_index = 0;

                if (line.empty())
                {
                    game_data.cards.push_back(temp_card);

                    temp_card = {};
                    card_index = 0;
                }
                else 
                {
                    assert(line.size() == 14);
                    temp_card.numbers[card_index][0] = to_int<uint16_t>(str_view{ &line[0 + 0], 2 });
                    temp_card.numbers[card_index][1] = to_int<uint16_t>(str_view{ &line[0 + 3], 2 });
                    temp_card.numbers[card_index][2] = to_int<uint16_t>(str_view{ &line[0 + 6], 2 });
                    temp_card.numbers[card_index][3] = to_int<uint16_t>(str_view{ &line[0 + 9], 2 });
                    temp_card.numbers[card_index][4] = to_int<uint16_t>(str_view{ &line[0 + 12], 2 });
                    card_index++;
                }
            }
        });

        return game_data;
    }

    size_t get_part1(game_t game_data)
    {
        for (uint8_t number: game_data.numbers)
        {
            for (card_t& card: game_data.cards)
                card.mark_number(number);

            for (const card_t& card: game_data.cards)
                if (card.has_win())
                    return number * card.get_unmarked_score();
        }
        
        assert(false);
        return 0;
    }

    size_t get_part2(game_t game_data)
    {
        for (uint8_t number: game_data.numbers)
        {
            for (card_t& card: game_data.cards)
                card.mark_number(number);

            if (game_data.cards.size() == 1)
            {
                const card_t& last_card = game_data.cards.front();
                if (last_card.has_win())
                {
                    // last_card.debug_print();
                    return number * last_card.get_unmarked_score();
                }
            }
            else
            {
                auto& v = game_data.cards;
                auto it = std::remove_if(v.begin(), v.end(), [](const card_t& card)
                {
                    return card.has_win();
                });
                v.erase(it, v.end());
            }
        }

        assert(false);
        return 0;
    }
}

output_t Day_2021_4::run_solution(str_view input) const
{
    const game_t game_data = parse(input);
    size_t part1 = get_part1(game_data);
    size_t part2 = get_part2(game_data);
    return { part1, part2 };
}

void Day_2021_4::run_tests() const
{
    char text1[] = 
        "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1\n"
        "\n"
        "22 13 17 11  0\n"
        " 8  2 23  4 24\n"
        "21  9 14 16  7\n"
        " 6 10  3 18  5\n"
        " 1 12 20 15 19\n"
        "\n"
        " 3 15  0  2 22\n"
        " 9 18 13 17  5\n"
        "19  8  7 25 23\n"
        "20 11 10 24  4\n"
        "14 21 16 12  6\n"
        "\n"
        "14 21 17 24  4\n"
        "10 16 15  9 19\n"
        "18  8 23 26 20\n"
        "22 11 13  6  5\n"
        " 2  0 12  3  7\n"
        "\n";
    const game_t game_data = parse(text1);
    assert(4512 == get_part1(game_data));
    assert(1924 == get_part2(game_data));
}
