#pragma once
#include "input_t.h"
#include <string>

void day01(const input_t&); void day01_test();
void day02(const input_t&); void day02_test();
void day03(const input_t&); void day03_test();
void day04(const input_t&); void day04_test();
void day05(const input_t&); void day05_test();
void day06(const input_t&); void day06_test();
void day07(const input_t&); void day07_test();
void day08(const input_t&); void day08_test();
void day09(const input_t&); void day09_test();
void day10(const input_t&); void day10_test();
void day11(const input_t&); void day11_test();
void day12(const input_t&); void day12_test();
void day13(const input_t&); void day13_test();
void day14(const input_t&); void day14_test();
void day15(const input_t&); void day15_test();
void day16(const input_t&); void day16_test();
void day17(const input_t&); void day17_test();
void day18(const input_t&); void day18_test();
void day19(const input_t&); void day19_test();
void day20(const input_t&); void day20_test();
void day21(const input_t&); void day21_test();
void day22(const input_t&); void day22_test();
void day23(const input_t&); void day23_test();
void day24(const input_t&); void day24_test();
void day25(const input_t&); void day25_test();

struct advent_t
{
    std::string day_id;
    std::string input_file;

    void (*fn_exec)(const input_t&);
    void (*fn_test)();
};
