#include "advent2020.h"

namespace
{
    template <typename T>
    T loop_around(T value, T min, T max)
    {
        if (value < min)
            return max;
        else if (value > max)
            return min;
        else
            return value;
    }

    template <typename T, size_t N>
    bool contains(std::array<T, N> data, T value)
    {
        for (T val : data)
            if (val == value)
                return true;

        return false;
    }

    struct cups_t
    {
        std::vector<size_t> links; // { idx: cup idx, val: next cup idx }

        void set_link(size_t from, size_t to)
        {
            links[from] = to;
        }

        void set_current(size_t cup)
        {
            links[0] = cup;
        }

        size_t get_current() const
        {
            return links[0];
        }

        size_t get_cup_count() const
        {
            return links.size() - 1;
        }

        std::array<size_t, 3> get_pickup() const
        {
            size_t first = links[get_current()];
            size_t second = links[first];
            size_t third = links[second];
            return { first, second, third };
        }

        size_t get_dest_cup(size_t current, const std::array<size_t, 3>& pickup) const
        {
            const size_t CUP_N = get_cup_count();
            size_t dest_cup = loop_around(current - 1, 1LU, CUP_N);
            while (contains(pickup, dest_cup))
            {
                dest_cup = loop_around(dest_cup - 1, 1LU, CUP_N);
            }
            return dest_cup;
        }

        [[maybe_unused]]
        void print() const
        {
            auto cup = get_current();
            printf("current: %zu (-> %zu)\n", cup, links[cup]);
            do
            {
                printf("%zu, ", cup);
                cup = links[cup];
            } while (cup != get_current());
            printf("(%zu) \n", cup);
        }

        size_t get_score_part1()
        {
            size_t score = 0;
            const size_t CUP_N = get_cup_count();
            auto cup = links[1];
            for (size_t d = 0; d < CUP_N - 1; ++d)
            {
                size_t mod = binpow(10, CUP_N - 2 - d);
                score += (cup * mod);
                cup = links[cup];
            }

            return score;
        }

        size_t get_score_part2()
        {
            size_t cup1 = links[1];
            size_t cup2 = links[cup1];
            return cup1 * cup2;
        }

        void grow_to(size_t max_index)
        {
            links.reserve(max_index + 1);

            const size_t current_max = get_cup_count();

            // update current loop-around link to continue instead
            for (size_t idx = 1; idx < links.size(); ++idx)
            {
                if (links[idx] == get_current())
                {
                    links[idx] = current_max + 1;
                    break;
                }
            }

            // fill in missing entries
            for (size_t idx = current_max + 1; idx <= max_index; ++idx)
            {
                links.push_back(idx + 1);
            }

            // update last link to loop around
            set_link(max_index, get_current());
        }
    };

    cups_t parse(input_t input)
    {
        std::vector<size_t> in;
        in.reserve(10);

        for (size_t i = 0; input.s[i] != '\n'; ++i)
            in.push_back((input.s[i] - '0'));

        cups_t out;
        out.links = std::vector<size_t>(10, 0);

        for (size_t i = 0; i < (in.size() - 1); ++i)
        {
            out.set_link(in[i], in[i + 1]);
        }

        out.set_link(in.back(), in.front());
        out.set_current(in.front());

        return out;
    }

    cups_t evaluate(cups_t cups, size_t moves)
    {
        for (size_t move = 0; move < moves; ++move)
        {
            auto current_cup = cups.get_current();
            auto pickup_cups = cups.get_pickup();
            auto dest_value = cups.get_dest_cup(current_cup, pickup_cups);

            std::array<size_t, 3> prev = {
                cups.links[current_cup],
                cups.links[dest_value],
                cups.links[pickup_cups[2]]
            };

            cups.links[current_cup] = prev[2];
            cups.links[dest_value] = prev[0];
            cups.links[pickup_cups[2]] = prev[1];

            cups.set_current(cups.links[current_cup]);
        }

        return cups;
    }
}

output_t day23(const input_t& input)
{
    auto out = parse(input);
    size_t part1 = evaluate(out, 100).get_score_part1();

    out.grow_to(1000000);
    size_t part2 = evaluate(out, 10000000).get_score_part2();

    return { part1, part2 };
}

void day23_test()
{
    char text1[] = "389125467\n";
    input_t test1 { text1, sizeof(text1) };

    auto out = parse(test1);

    assert(92658374 == evaluate(out, 10).get_score_part1());
    assert(67384529 == evaluate(out, 100).get_score_part1());

    out.grow_to(1000000);
    assert(149245887792 == evaluate(out, 10000000).get_score_part2());
}
