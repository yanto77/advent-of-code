# Advent Of Code 2020, notes

[[./img/aoc2020_50stars.jpeg]]

[[__TOC__]]

## Intro

[Advent of Code](https://adventofcode.com/) has caught my attention a couple years before, but I did not pick it up at that point. The 2020 event was the first one I attended and, after completing it, I'm happy with the time spent on it. I did not approach it as a competition: my initial commit dates 8 Dec 2020 and the 50* commit -- 6 Mar 2021. There were multiple days when I was stuck on searching for the solution, but most of these days I didn't touch this project at all.

Before starting, I expected the solutions to the puzzles to be found relatively easily and my mind was set on learning to optimize the solutions to be fast, inspired by projects such as [Voltara/advent2018-fast](https://github.com/Voltara/advent2018-fast). Quickly, I found out that the learning curve is not as easy as initially expected. To be proficient in solving the puzzles in reasonable time, one is expected to utilize many different data structures and algorithms.

In the end, I was able to write solutions in C++ to all puzzles, adding up to 2 seconds of execution time on my hardware. Comparing against [Voltara/advent2020-fast](https://github.com/Voltara/advent2020-fast) (compiled and executed on the same hardware), most of my solutions are 1-5 times slower, while 8 solutions are slower by 20-14000 times. I am happy with my results, but there's definitely possibilities to improve the solutions even more.

Overall, I've enjoyed the process of creating the solutions to the puzzles. The puzzles themselves can be simplified to "construct a function that produces required output with given input", however, the backstory and details do add additional flavor to the exercise. There were multiple days I was stuck and couldn't do without reading and studying the solutions of others (e.g., [Day 13](https://fasterthanli.me/series/advent-of-code-2020/part-13), [Day 19](https://github.com/Voltara/advent2020-fast/blob/main/src/day19.cpp)).

Below, I would like highlight couple of days that surprised me the most.

## Day 5

The interesting part of completing a day is to compare own solutions against others, reading how other people approached the problem and how they wrote the code. The solutions are often composed from inter-changeable parts, allowing experimentation.

One of such experiments is presented below. From the solutions I've found, some combined the if-clauses into one part (`convert_line1`) and others split them into own parts (`convert_line2`). The effect of this was quite surprising to me -- using `convert_line2` the solution takes 14-16 μs, which is approx. 80% less than if `convert_line1` is used (77-78 μs).

```cpp
uint16_t convert_line1(const sv& line) // e.g. "BFFFBBFRRR"
{
    uint16_t result = 0;

    for (uint8_t i = 0; i < 10; ++i)
        if (line[i] == 'B' || line[i] == 'R')
            set_bit(result, (9 - i));

    return result;
}

uint16_t convert_line2(const sv& line)
{
    uint16_t result = 0;

    for (uint8_t i = 0; i < 7; ++i)
        if (line[i] == 'B')
            set_bit(result, (9 - i));

    for (uint8_t i = 0; i < 3; ++i)
        if (line[7 + i] == 'R')
            set_bit(result, (2 - i));

    return result;
}
```

In a sense, the difference is understable, because in `convert_line1` some conditions are evaluated which are definitely false -- `line[i] == 'R'` for the first 7 chars and `line[i] == 'B'` for the last 3 chars. Assuming uniform distribution of input values, there will be 3.5 + 3 unnecessary comparisons done for every input. Thus, switching to `convert_line2` reduces the number of comparisons by 40% (from 16.5 to 10).

However, the number of executed instruction doesn't in itself explain the runtime difference. Statistics recorded with `perf stat -r 1000` show interestingly consistent number of branches and the difference in the number of branches between `convert_line1` and `convert_line2` is approx. 9000. Dividing by 6.5 gives 1384, which is quite close to the 1024 lines the program is processing. Thus, it can probably be concluded than `convert_line2` is better from branch prediction point of view as well.

## Day 23

The contrast between the solutions for both parts for Day 23 was probably the most memorable for me. The puzzle gives a list of 10 numbers and asks to re-arrange them based on specific rules. Well, I saw small input and I saw parts moving around, so the solution I came up with for the part 1 is to _copy_ things around. With this approach, each part must be carefully copied in correct order: first the current cup, then all cups up to the destination cup, then the destination cup itself, then 3 pickup cups, and finally the cups after the destination cup. As you might imagine, doing this with 10 numbers for 100 loops is very fast. One might even think that this would an optimal solution!

The part 2 grows the input size to 1,000,000 and asks to loop around 10,000,000 times. The approach of copying things around between two buffers blows up to _infinite_ runtime, assumingly, as I didn't care to wait. The important part to understand here was that instead of thinking about cups themselves, one must consider the links between them instead! I evaluated the situation in a spreadsheet and came to the conclusion that only three links are relevant in each loop, reducing the number of copies from 1,000,000 to 3! The effect of this is that the runtime of the solution stays around comfortable 300 ms.

[[img/day23.png]]


Focusing on the value swapping, my initial approach was to save the values into temporary array and write them back in different order (see `swap1` below). Reading through solutions of others, I found out about `std::exchange` and `std::swap`, so I experimented with variations `swap2` and `swap3`. In the end, the effects between them are seem to be quite small and the output assembly is mostly similar (see https://godbolt.org/z/sxo97h). Swapping using `std::swap` has two extra `mov`s, as it does one extra read and write that we know is unnecessary. On the other hand, `swap1` and `swap3` produce similar output with exception of two differences: (1)  the `std::exchange` version re-uses `eax`, while `swap1` uses three different registers, and (2)

```cpp
#include <array>
// swap (a, b, c) into (c, a, b)

void swap1(std::array<int, 10>& data, int a, int b, int c)
{
    std::array<int, 3> prev = {data[a], data[b], data[c]};
    data[a] = prev[2];
    data[b] = prev[0];
    data[c] = prev[1];
}

void swap2(std::array<int, 10>& data, int a, int b, int c)
{
    std::swap(data[a], data[b]);
    std::swap(data[a], data[c]);
}

void swap3(std::array<int, 10>& data, int a, int b, int c)
{
    auto x = std::exchange(data[a], data[c]);
    auto y = std::exchange(data[b], x);
    data[c] = y;
}
```

## Optimizations

The background I have for the optimization is the course I attended in the university -- [Programming Parallel Computers](http://ppc.cs.aalto.fi). Briefly, the course starts with creating a relatively simple program and going through different stages of optimization on the same program to see the runtime drop from 99 seconds to 0.7 seconds. As the course page describes, "achieving good performance on a modern CPU is much more than simply using multiple threads".

To collect benchmarking statistics, I've used [`perf`](https://perf.wiki.kernel.org/index.php/Main_Page), but there are also [other tools](https://stackoverflow.com/a/60265409). Using `perf record` to see the parts that take most time is a great first step. From the numbers I've seen usually, the trace frequency has been ~60 kHz for me, yielding resolution of the events ~16 μs. `perf stat` outputs a set of statistics, some of which are above my level of understanding, but [great answers](https://stackoverflow.com/q/22165299) on SO help with that.

Overall, I've learned multiple things when attempting to optimize the solutions.

- Using multiple threads (e.g. with openmp) is often unnecessary, as the solutions are often in the microsecond range. To put it in different words, if the solution for any given day takes too long to execute, there are other things to look for to improve the runtime outside of multithreading. From some simple measurements, it seems that multithreading becomes more relevant in the millisecond range.

- Profiling with resolution of 16 μs may yield too crude results. For six solutions of mine such resolution yields less than 5 events per execution, and some of these solutions can be made faster by a factor of 2-5, at least comparing to solutions from other people. As such, `perf` seems to be relevant only up to a certain point.

- Limiting integers to specific sizes was less of a thing than I expected, that is, using `int` versus `int8_t`/`int16_t`/etc. During Day 4, the data struct in my solution could be  packed from 56 bytes to 40 bytes (-28%), which sounds great on paper. However, the amount of data the solution is processing is so small, the amount of saved memory is 4640 bytes in total -- the memory size is not really an issue.

- In general, the compiler does so many things that without measurements it is not trivial to say if a change is going to improve/worsen the performance. For example, in Day 11 solution I've made a change of extracting inner loop that calculates nearest occupied seats (for part 1 only) into its own method. For some reason, moving the code into different place alone reduced the runtime from 7700 μs to 5900 μs (-1800 μs, or -23%).

## Reflections

Finally, I would like to reflect on this experience from point of view of learning. I feel it is important that the puzzles do not limit the user in any way, allowing to use many different data structures and algorithms, thus making the bruteforce solutions (often) as valid and applicable as math-y solutions. In addition, Eric Wastl mentioned during [FOSDEM talk](https://ftp.osuosl.org/pub/fosdem/2021/D.perl/adventofcode.webm) (around 20:25) that the two-part structure of the puzzles intentionally attempts to simulate real world environments with code maintenance and end-user wanting new things out of the same code.

Consequently, one of the major areas I've personally felt noticeable improvement was in using C++ language itself. Specifically:

- During initial days I was slightly lost with parsing of the data and used a [regex library](https://github.com/hanickadot/compile-time-regular-expressions) for that (e.g. Day 2), while later on I moved into using a set of common parsing helpers and state machines for more complex input format (such as Days 4, 7, 14, 16).

- In addition, it was easy to experiment with some C++ features that were new for me, such as `<string_view>`, `<bitset>`, `constexpr` and `static_assert`s. For a while I've read from different source that `std::string_view` should be preferred over `std::string` in certain situations, but didn't had the chance to actually try it on something non-trivial. AOC puzzles here take the sweet spot between [reference examples](https://en.cppreference.com/w/cpp/header/string_view), which tell "how" but skip on "why" and "where", and pre-existing codebases, where adding a new feature may not be wise, especially when caveats are not well understood.

- Lastly, as the programming is often a community effort, it is a great exercise to read and understand the solutions of others, e.g. from the [reddit community](https://www.reddit.com/r/adventofcode/). When doing that, I've found out that the solutions I wrote are quite verbose. It was very surprising to find how well other solutions were able to express similar concepts with much more clarity and less cruft.

