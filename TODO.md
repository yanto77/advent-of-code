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

graphs
    vertex list + link list (+ weights) -> 2 sets of (dyn) arrays
    adjancency matrix (v^2) -> static array 


## Project specific 

* Script to generate day*.cpp stubs.
* Script to download input/day*.txt files.