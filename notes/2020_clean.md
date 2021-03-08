# Advent Of Code 2020, notes

## Intro

[Advent of Code](https://adventofcode.com/) has caught my attention a couple years before, but I did not pick it up at that point. The 2020 event was the first one I attended and, after completing it, I would say am I happy with the time spent on it. I did not approach it as a competition: my initial commit dates 8 Dec 2020 and the 50* commit -- 6 Mar 2021. There were multiple days when I was stuck on searching for the solution, but most of these days I didn't touch this project at all. 

Before starting, I expected the solutions to the puzzles to be found relatively easily and my mind was set on learning to optimize the solutions to be fast, inspired by projects such as [Voltara/advent2018-fast](https://github.com/Voltara/advent2018-fast). Quickly, I found out that the learning curve is not as easy as initially expected. To be proficient in solving the puzzles in reasonable time, one is expected to utilize many different data structures and algorithms.  

In the end, I was able to write solutions in C++ to all puzzles, adding up to 2 seconds of execution time on my hardware. Comparing against [Voltara/advent2020-fast](https://github.com/Voltara/advent2020-fast) (compiled and executed on the same hardware), most of my solutions are 1-5 times slower, while 8 solutions are slower by 20-14000 times.

## Days [TODO]

overview: there are many days i've liked creating a solution for, and there were multiple days i was stuck and couldn't do without help from comments of others from reddit community.

i would like to highlight multiple days that surprised me the most. or some explicit learnings.


### Day 5 [TODO]

- [TODO] a binary tree extended by a second binary tree, are not two trees, but the one and the same tree.

- [TODO] comparing against other solutions, there were some interesting differences. but one of the major things found was difference between having two if-clauses or combining them into one. that by itself takes solution bewteen 90 μs and 20-25 μs (~75%). 


### Day 11 [TODO]

- initial solution for part 1 took 7700 μs. extracting method to compute nearest occupied seats (for part 1 only) yielded 1800 μs reduction in runtime. why though?

- construct simple repro?

### Day 13 [TODO]

- first one i was completely lost in. the blog post from fasterthanli.me is the greatest thing. learned a lot here!

### Day 19 [TODO]

- had probably the most notes from what i had for other days.

### Day 23 [TODO]

- going with the simplest approach of translating the rules into code yields us the need to consider copying following data segments over after each move: current index, values before destination value, the destination value itself, pickup cups, and then post-destination values. copying all of those works very well with the part 1, and expectedly fails with part 2. 

- it is such a great feeling to understand at some point that 99% of the operations you are doing are actually unnecessary and the solution resolves to updating three links only, dropping the runtime from "too long to wait for" to approx. 300 ms.

- initially i was doing std::array copy and assign in different order. there are some other solutions using std::exchange, but also std::swap seems to do something similar. there are some notes related to asm analysis.


## Optimizations

The background I have for the optimization is the course I attended in the university -- [Programming Parallel Computers](http://ppc.cs.aalto.fi). Briefly, the course starts with creating a relatively simple program and going through different stages of optimization on the same program to see the runtime drop from 99 seconds to 0.7 seconds. As the course page describes, "achieving good performance on a modern CPU is much more than simply using multiple threads".

To collect benchmarking statistics, I've used [`perf`](https://perf.wiki.kernel.org/index.php/Main_Page), but there are also [other tools](https://stackoverflow.com/a/60265409). Using `perf record` to see the parts that take most time is a great first step. From the numbers I've seen usually, the trace frequency has been ~60 kHz for me, yielding resolution of the events ~16 μs. `perf stat` outputs a set of statistics, some of which are above my level of understanding, but [great answers](https://stackoverflow.com/q/22165299) on SO help with that.

Overall, I've found out multiple things that surprised me to some degree.

- Using multiple threads (e.g. with openmp) is often unnecessary, as the solutions are often in the microsecond range. To put it differently, if the solution for any given day takes too long to execute, there are other things to look for to improve the runtime outside of multithreading. From some simple measurements, it seems that multithreading becomes more relevant in the millisecond range.

- Profiling with resolution of 16 μs may yield too crude results. For six solutions of mine such resolution yields less than 5 events per execution, and some of these solutions can be made faster by a factor of 2-5. As such, `perf` seems to be relevant only up to a certain point.

- Limiting integers to specific sizes was less of a thing than I expected, that is, using `int` versus `int8_t`/`int16_t`/etc. During Day 4, the data struct in my solution could be  packed from 56 bytes to 40 bytes (-28%), which sounds great on paper. However, the amount of data the solution is processing is so small, the amount of saved memory is 4640 bytes in total -- the memory size is not really an issue.


## Reflections [TODO]

Overall, the learning comes from doing, but also from reflection. So what did I learn? Results of this effort? 


One of the major areas I've felt noticeable improvement in was using C++ language itself. Some examples:

- The initial days I was slightly lost with parsing of the data and used a [regex library](https://github.com/hanickadot/compile-time-regular-expressions) for that (e.g. Day 2), while later on I moved into using a set of common parsing helpers and state machines for more complex input format (such as Days 4, 7, 14, 16).

- In addition, it was easy to experiment with some C++ features, such as `<string_view>`, `<bitset>`, `constexpr` and `static_assert`s. For a while I've been reading from different source that one should prefer `std::string_view` over `std::string` (in certain situations).



- refactoring
    - the FOSDEM talk of Eric Wastl touched the subject of refactoring the solutions. so it's not just "write and forget", but some effort needs to be spent on maintaining the code for the 1st part.
- learning from others:
    - reddit community. reading and understanding solutions of others.
    - the solutions i write are quite verbose. 
