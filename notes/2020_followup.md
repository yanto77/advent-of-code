
day 1.
    - previously control flow was handled with breaks and ifs. separating the part in its own function allows using single `return` instead, thus reducing the need to check for stop-conditions in each loop.
        -> the numbers are now closer to 100 than 200.

    - in addition, within part2 it's possible to break early in conditions when (i + j) exceeds the target sum. optionally, smallest value can be added to that condition, with minor effect.
        -> this drops exec time to 75-80 range.

day 2.

    - as found out later, regular expressions are little too hardcore for this input. removing ctre dependency (in favor of own simpler parsing methods) drops the runtime from 530 to 75.

    - replacing std::count_if with simple for-loop counter didn't yield any benefit, nor any drawbacks.


day 3.
    - replace `std::array<bool, COL_N>` with single `uint32_t` ? COL_N is 31, so i guess it was intended.

        - original:        60 +- 4
        - uint32_t way:    60 +- 2.5

        -> less memory (10013 -> 1292), but no noticeable difference on perf

    - added simple stack timer. it shows that 70% is spent in parsing method, not in the calculation method.
        - replacing `line.size()` with constant 31 doesn't help.

    - TODO: the next would be to understand the AVX2 parts from `to_bits` in voltaras solution. changing that part adds bunch of (irrelevant?) warnings and also reduces the runtime 60 -> 12.

day 9.

    - finding continuous set is the most costly operation atm, taking approx 240 μs or 80% of the whole duration. the approach it is using is for-loop in a for-loop.
        -> updating to the sliding window approach (see https://timvisee.com/blog/solving-aoc-2020-in-under-a-second/ ) removes the main part of the cost - it now takes 0-1 μs. the whole solution now takes only 120 μs on average (best case being around 55 μs).

        -> why such effect? probably because of better utilization of cache lines (in addition to reduced number of operations). with double for-loop we are jumping across the data in non-optimal order. with sliding window, we are mostly working with data in the same or nearest cache lines. the effect of loading cache lines and dropping them is reduced to great extent.

    - one thing i forgot is to calculate only half of the matrix, instead of checking all pairs
        -> minor benefits here!

    - the next candidate for optimization would be get_invalid_num(), which finds the invalid number by calculating two-element sums with sliding window. the function takes approx 50% of the runtime.

        - one thing we can notice is that for subsequent numbers we might be recalculating the same two-piece sums that we did just one round ago.

        - analyzing the specific input shows that there are 28585 sum operations done, but only 9565 of then are unique. the repetition distribution looks like:

            1       3231
            2-5     4973
            6-9     1289
            10-18   72

        - the number of operation could be reduced by focusing on the sliding window, instead of the last element. with the sum values we compute within this sliding window, all subsequent elements could be checked for validity, as opposed to checking only one. skipping over large portions of valid elements would allow us to skip the summation operations as well.
            -> the effect on the operation could is visible -> 11902 operations left, so 16683 (~58%) have been skipped. it doesn't help the runtime though, as we need additional run-time bookkeeping, and for now i am using dynamic std::vector<bool> to save the validity state.

        - what about using std::array<bool> (or uint32_t) within the loop and having it non-dynamically?
            -> doesn't help.

        - what about parse method?? to_int<> could be tested here nicely
            - prev std::from_chars() -> 40 us
            - next conv from voltara -> 30 us

        -> all of these changes bring us average 90 us!


day22:

    - https://www.reddit.com/r/adventofcode/comments/khyjgv/2020_day_22_solutions/ggrht8v
        -> "Oh, you literally just win with the high card."

    -> https://www.reddit.com/r/adventofcode/comments/khyjgv/2020_day_22_solutions/ggpcsnd
        -> implementation gives 26x gains right away (1100 ms -> 40 ms).


TODO:
    day9: find_continuous_set()
    day13: extract common stuff, like the equation solvers


    memory constrained: day15, day23
        > sudo sysctl -w vm.nr_hugepages=64