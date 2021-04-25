
day 1.
    - previously control flow was handled with breaks and ifs. separating the part in its own function allows using single `return` instead, thus reducing the need to check for stop-conditions in each loop.
        -> the numbers are now closer to 100 than 200.

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