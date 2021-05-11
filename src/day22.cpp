#include "advent2020.h"

struct decks_t
{
    std::deque<int> p1;
    std::deque<int> p2;

    std::tuple<int, int> pop()
    {
        int c1 = p1.front();
        int c2 = p2.front();
        p1.pop_front();
        p2.pop_front();
        return { c1, c2 };
    }

    void push(int c1, int c2, bool p1_wins)
    {
        if (p1_wins)
        {
            p1.push_back(c1);
            p1.push_back(c2);
        }
        else
        {
            p2.push_back(c2);
            p2.push_back(c1);
        }
    }

    std::pair<size_t, bool> get_score()
    {
        size_t score = 0;

        bool p1_win = !p1.empty();
        const auto& deck = p1_win ? p1 : p2;

        const size_t N = deck.size();
        for (size_t i = 0; i < N; ++i)
        {
            score += deck[i] * (N - i);
        }
        return { score, p1_win };
    }

    bool check_p1_autowin() const
    {
        // Ref: https://www.reddit.com/r/adventofcode/comments/khyjgv/2020_day_22_solutions/ggpcsnd
        // "During a sub game, if we see that the player 1 has the card with the
        // highest number and the value of that card is more than the length of
        // both decks combined, then we can declare Player 1 as winner! This will
        // significantly reduce the recursion space."

        int max_p1 = *std::max_element(p1.begin(), p1.end());
        int max_p2 = *std::max_element(p2.begin(), p2.end());
        return max_p1 > max_p2 && (max_p1 > (p1.size() + p2.size() - 2));
    }

    bool operator==(const decks_t& other) const
    {
        return (p1 == other.p1 && p2 == other.p2);
    }

    bool operator<(const decks_t& other) const
    {
        return (p1 < other.p1 && p2 < other.p2);
    }
};

namespace std
{
    template <>
    struct hash<decks_t>
    {
        size_t operator()(const decks_t& k) const
        {
            return 33 * djb_hash(k.p1) ^ djb_hash(k.p2);
        }
    };
}

namespace
{
    decks_t parse_decks(const input_t& input)
    {
        decks_t decks;

        bool add_to_p1 = true;
        bool skip_line = true;
        parse_input(input, [&](const sv& line)
        {
            if (skip_line)
            {
                skip_line = false;
                return;
            }

            if (line.empty())
            {
                skip_line = true;
                add_to_p1 = false;
            }
            else if (add_to_p1)
            {
                decks.p1.push_back(to_int<int>(line));
            }
            else
            {
                decks.p2.push_back(to_int<int>(line));
            }
        });

        return decks;
    }

    std::tuple<size_t, bool> play_game(decks_t decks)
    {
        while (!decks.p1.empty() && !decks.p2.empty())
        {
            auto [c1, c2] = decks.pop();
            decks.push(c1, c2, c1 > c2);
        }

        return decks.get_score();
    }

    std::tuple<size_t, bool> play_game_recursive(decks_t decks, bool is_sub)
    {
        if (is_sub && decks.check_p1_autowin())
        {
            return { 0, true };
        }

        std::unordered_map<decks_t, bool> known_states;
        while (!decks.p1.empty() && !decks.p2.empty())
        {
            auto [it, inserted] = known_states.insert({ decks, true });
            if (!inserted)
            {
                return { 0, true };
            }

            auto [c1, c2] = decks.pop();
            if (c1 > decks.p1.size() || c2 > decks.p2.size())
            {
                /// Play normal game
                decks.push(c1, c2, c1 > c2);
            }
            else
            {
                /// Play sub-game
                decks_t sub_decks;

                for (size_t i = 0; i < c1; ++i)
                    sub_decks.p1.push_back(decks.p1[i]);

                for (size_t i = 0; i < c2; ++i)
                    sub_decks.p2.push_back(decks.p2[i]);

                auto [subscore, p1_subwin] = play_game_recursive(sub_decks, true);
                decks.push(c1, c2, p1_subwin);
            }
        }

        return decks.get_score();
    }
}

output_t day22(const input_t& input)
{
    auto decks = parse_decks(input);
    auto [part1, p1_won_part1] = play_game(decks);
    auto [part2, p1_won_part2] = play_game_recursive(decks, false);
    return { part1, part2 };
}

void day22_test()
{
    {
        char text1[] = "Player 1:\n"
                       "9\n"
                       "2\n"
                       "6\n"
                       "3\n"
                       "1\n"
                       "\n"
                       "Player 2:\n"
                       "5\n"
                       "8\n"
                       "4\n"
                       "7\n"
                       "10\n";
        input_t test1 { text1, sizeof(text1) };

        auto decks = parse_decks(test1);
        auto [part1, p1_won_part1] = play_game(decks);
        auto [part2, p1_won_part2] = play_game_recursive(decks, false);

        assert(part1 == 306);
        assert(!p1_won_part1);
        assert(part2 == 291);
        assert(!p1_won_part2);
    }

    {
        // Infinite game test!
        char text1[] = "Player 1:\n"
                       "43\n"
                       "19\n"
                       "\n"
                       "Player 2:\n"
                       "2\n"
                       "29\n"
                       "14\n";
        input_t test1 { text1, sizeof(text1) };

        auto decks = parse_decks(test1);
        auto [part2, p1_won_part2] = play_game_recursive(decks, false);
        assert(part2 == 0);
        assert(p1_won_part2);
    }
}
