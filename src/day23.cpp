#include "advent2020.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

#define printf(...)
#define print_row(...)
constexpr auto SLEEP_AMOUNT = 1ms;

namespace
{
    //constexpr size_t CUP_N = 9;
    typedef std::vector<size_t> cups_t;

    cups_t parse(input_t input)
    {
        cups_t out;
        out.reserve(10);

        for (size_t i = 0; input.s[i] != '\n'; ++i)
            out.push_back((input.s[i] - '0'));

        return out;
    }

    template <typename T, size_t N>
    bool contains(std::array<T, N> data, T value)
    {
        for (T val: data)
            if (val == value)
                return true;

        return false;
    }

    // template <typename T>
    // bool contains(std::vector<T> data, T value)
    // {
    //     for (T val: data)
    //         if (val == value)
    //             return true;

    //     return false;
    // }

    template <typename T>
    size_t index_of(std::vector<T> data, T value)
    {
        const size_t N = data.size();
        for (size_t i = 0; i < N; ++i)
            if (data[i] == value)
                return i;

        return 0;
    }

    cups_t evaluate(cups_t cups, size_t moves)
    {
        const size_t CUP_N = cups.size();
        assert(CUP_N != 0);

        cups_t copy = cups;
        printf("evaluate!\n");

        for (size_t move = 0; move < moves; ++move)
        {
            std::cout << "move: " << move << "\t\t\t\r";
            copy = cups;
            size_t idx = move % CUP_N;
            printf("-- move %zu --\n", move + 1);

            printf("cups: "); print_row(cups);
            printf("current: %lu\n", cups[idx]);

            std::array<size_t, 3> pickup {};
            for (size_t i = 0; i < 3; ++i)
            {
                size_t read_idx = (idx + 1 + i) % CUP_N;
                pickup[i] = cups[read_idx];
            }
            printf("pickup: "); print_row(pickup);

            size_t dest_value = (cups[idx] - 1);
            if (dest_value <= 0) dest_value = CUP_N;

            printf(" .. dest_value? %lu\n", dest_value);
            while (contains(pickup, dest_value))
            {
                --dest_value;
                if (dest_value <= 0) dest_value = CUP_N;
                printf(" .. dest_value? %lu\n", dest_value);
            }
            printf("dest_value: %lu\n", dest_value);

            // Starting from idx:
            size_t read_idx = idx;
            size_t write_idx = idx;

            auto advance_both_idx = [&]()
            {
                read_idx = (read_idx + 1) % CUP_N;
                if (cups[read_idx] == pickup[0])
                    read_idx = (read_idx + 3) % CUP_N;
                write_idx = (write_idx + 1) % CUP_N;
            };

            // 1. write down current to idx
            printf("- 1. reading (%lu), writing to %zu\n", cups[read_idx], write_idx);
            copy[write_idx] = cups[read_idx];
            advance_both_idx();

            // 2. pre-destination
            while (cups[read_idx] != dest_value)
            {
                printf("- 2. reading (%lu), writing to %zu\n", cups[read_idx], write_idx);
                copy[write_idx] = cups[read_idx];
                advance_both_idx();
                std::this_thread::sleep_for(SLEEP_AMOUNT);
            }

            // 2.1 destination itself
            printf("- 2.1 reading (%lu), writing to %zu\n", cups[read_idx], write_idx);
            copy[write_idx] = dest_value;
            advance_both_idx();

            // 3. pickup cups (3 values)
            for (auto picked_value: pickup)
            {
                printf("- 3. reading pickup (%lu), writing to %zu\n", picked_value, write_idx);
                copy[write_idx] = picked_value;
                write_idx = (write_idx + 1) % CUP_N;
                std::this_thread::sleep_for(SLEEP_AMOUNT);
            }

            // 4. post-destination (and pickup cups)
            while (cups[read_idx] != cups[idx])
            {
                printf("- 4. reading (%lu), writing to %zu\n", cups[read_idx], write_idx);
                copy[write_idx] = cups[read_idx];
                advance_both_idx();
                std::this_thread::sleep_for(SLEEP_AMOUNT);
            }

            printf("-> result: "); print_row(copy);
            cups = copy;
        }

        std::cout << "\nend!\n";

        return cups;
    }

    size_t get_part1_score(const cups_t& cups)
    {
        const size_t CUP_N = cups.size();
        size_t score = 0;
        size_t idx = index_of(cups, 1LU) + 1;
        for (size_t d = 0; d < CUP_N - 1; ++d)
        {
            size_t val = cups[(idx + d) % CUP_N];
            size_t mod = binpow(10, CUP_N - 2 - d);
            printf("score += %zu * %zu\n", val, mod);
            score += (val * mod);
        }

        return score;
    }

    // size_t get_part2_score()
    // {
    //     return 0;
    // }
}

output_t day23(const input_t& input)
{
    auto out = parse(input);
    auto cups = evaluate(out, 100);
    size_t part1 = get_part1_score(cups);

    return { part1, 0 };
}

void day23_test()
{
    char text1[] = "389125467\n";
    input_t test1 { text1, sizeof(text1) };

    auto out = parse(test1);
    // print_row(out);

    // assert(92658374 == get_part1_score(evaluate(out, 10)));
    // assert(67384529 == get_part1_score(evaluate(out, 100)));
}
