#include "setup/all.h"

ADVENT_DAY(2021, 8, 301, 908067);

namespace
{
    typedef std::bitset<7> segments_t; // bit for each display segment (random order!)
    typedef std::array<segments_t, 10> signal_t; // input/output signals as they come
    typedef std::array<segments_t, 10> signal_map_t; // { index (digit) -> signal value }
    typedef std::array<uint8_t, 4> signal_out_t; // output signal with proper digits
    struct signal_pair_t { signal_t in; signal_t out; };

    void parse_line(const sv& line, signal_pair_t* result)
    {
        bool left = true;
        int index = 0;

        for (char ch: line)
        {
            if (ch == ' ')
            {
                index++;
            }
            else if (ch == '|')
            {
                left = false;
                index = -1;
            }
            else
            {
                const int bit = static_cast<int>(ch - 'a');

                if (left)
                    result->in[index].set(bit);
                else
                    result->out[index].set(bit);
            }
        }
    }

    signal_map_t process_input_sig(signal_t digits)
    {
        signal_map_t map {};

        auto _set = [&](uint8_t index, segments_t& seg)
        {
            map[index] = seg;
            seg = 0;
        };

        // Map {1,4,7,8} based on the unique bit sizes
        for (segments_t& segments: digits)
        {
            const size_t bits = segments.count();

            if (bits == 2)
                _set(1, segments);
            else if (bits == 3)
                _set(7, segments);
            else if (bits == 4)
                _set(4, segments);
            else if (bits == 7)
                _set(8, segments);
        }

        // Map {9}
        for (segments_t& segments: digits)
            if ((segments & map[4]) == map[4])
                _set(9, segments);

        // Map {0,3}
        for (segments_t& segments: digits)
        {
            const bool eq7 = (segments & map[7]) == map[7];
            if (eq7 && segments.count() == 5)
                _set(3, segments);
            if (eq7 && segments.count() == 6)
                _set(0, segments);
        }

        // Map {6}
        for (segments_t& segments: digits)
            if (segments.count() == 6)
                _set(6, segments);

        // Map {2,5}
        for (segments_t& segments: digits)
        {
            const segments_t res = (segments & map[4]);
            if (res.count() == 2)
                _set(2, segments);
            else if (res.count() == 3)
                _set(5, segments);
        }

        return map;
    }

    std::pair<size_t, size_t> compute_answers(const input_t& input)
    {
        size_t part1 = 0;
        size_t part2 = 0;

        parse_input(input, [&](const sv& line)
        {
            signal_pair_t signals;
            parse_line(line, &signals);

            const signal_map_t& map = process_input_sig(signals.in);

            // Translate output signal into proper numbers
            signal_out_t out_num {};
            for (uint8_t i = 0; i < 4; i++) // for each output number
                for (uint8_t j = 0; j < 10; j++) // for each digit in map
                    if (map[j] == signals.out[i])
                        out_num[i] = j;

            // Part 1
            for (uint8_t v: out_num)
                if (v == 1 || v == 4 || v == 7 || v == 8)
                    part1++;

            // Part2
            part2 += out_num[0] * 1000;
            part2 += out_num[1] * 100;
            part2 += out_num[2] * 10;
            part2 += out_num[3] * 1;
        });

        return { part1, part2 };
    }
}

output_t Day_2021_8::run_solution(const input_t& input) const
{
    const auto& [part1, part2] = compute_answers(input);
    return { part1, part2 };
}

void Day_2021_8::run_tests() const
{
    char text1[] =
        "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe\n"
        "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc\n"
        "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg\n"
        "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb\n"
        "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea\n"
        "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb\n"
        "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe\n"
        "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef\n"
        "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb\n"
        "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce\n";
    input_t test1 { text1, sizeof(text1) };

    const auto& [part1, part2] = compute_answers(test1);
    assert(part1 == 26);
    assert(part2 == 61229);
}
