
## bitset / bitmasks

examples: 2020/03, 2020/04, 2020/05, 2020/06, 2020/14, 2020/16, 2020/19, 2021/03

it seems that `std::bitset` is useful mostly for single/full value checking. that is, when using bitset to check single bits, or the bitset as a whole value (all/none bits). by whole value, a value divisible by 8 bits is implied.

for situations with sliding window (checking multiple individual bits), it seems better to use `std::array<bool>`, which takes 8x memory, but the generated code is smaller and much faster (based on 2020/05). this seems due to the fact that computer can only load 8 bits at a time, so when loading unaligned bits from the bitset, it loads the full 8 bits and zeroes all unnecessary bits. in case of `std::array<bool>`, this is not necessary, because 1 bool takes 8 bits that can be loaded (and compared) directly, without unnecessary zeroing.


todo: 2020/14, 2020/16, 2020/19, 2021/03

380-420