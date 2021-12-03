# TODO

Some ideas for additional things.

## Code specific

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


## Project specific 

* Script to generate day*.cpp stubs.
* Script to download input/day*.txt files.