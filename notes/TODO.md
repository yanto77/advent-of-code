# TODO

Some ideas for additional things.

## Code specific

* Experiment with custom hashmaps:
    - https://github.com/JCash/containers
    - https://codingtidbit.com/2020/01/02/c14-20-heterogeneous-lookup-benchmark/
    - https://probablydance.com/2017/02/26/i-wrote-the-fastest-hashtable/
    - https://artificial-mind.net/blog/2021/10/09/unordered-map-badness
    - https://www.andreinc.net/2021/10/02/implementing-hash-tables-in-c-part-1
    - https://www.codeproject.com/Articles/5314559/Multi-Level-Cache-that-Does-Up-To-A-Billion-Lookup

* Extract more standardized algorithms outside the day-implementations.

### algo-header

count_occurences
    2020/02 (from string_view)
    2020/24 (from 2d bool array)

parse number rows into vector<int>
    2020/09
    2020/10

line translation
    mapping a char line into std::vector<> or bitmask, 
        using some dict rules {e.g. 'L': enum::LINE, ..}
    2020/03 (bitmask, single char)
    2020/05 (bitmask, two chars)
    2020/11 (vector<enum>, three chars)
    2020/14 (multiple bitmasks, three chars)
    2020/17 (vector<>, one char)
    2020/20 (multiple bitmasks, single char) -> the monster

tree traversals
    2020/07

matching 2d array onto other array
    2020/20 (monster array onto map)

permutations:
    2020/14

vector contains value
    2020/23


## bitset / bitmasks

examples: 2020/03, 2020/04, 2020/05, 2020/06, 2020/14, 2020/16, 2020/19, 2021/03

it seems that `std::bitset` is useful mostly for single/full value checking. that is, when using bitset to check single bits, or the bitset as a whole value (all/none bits). by whole value, a value divisible by 8 bits is implied.

for situations with sliding window (checking multiple individual bits), it seems better to use `std::array<bool>`, which takes 8x memory, but the generated code is smaller and much faster (based on 2020/05). this seems due to the fact that computer can only load 8 bits at a time, so when loading unaligned bits from the bitset, it loads the full 8 bits and zeroes all unnecessary bits. in case of `std::array<bool>`, this is not necessary, because 1 bool takes 8 bits that can be loaded (and compared) directly, without unnecessary zeroing.


todo: 2020/14, 2020/16, 2020/19, 2021/03
