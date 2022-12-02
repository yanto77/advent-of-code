#pragma once
#include <memory>
#include <string>
#include <map>
#include "output_t.h"

// Interface class representing solutions to each day/problem
class IDay
{
  public:
    IDay() = default;
    virtual ~IDay() = default;

    // Return valid output (the correct answers)
    virtual output_t get_valid() const = 0;

    // Runs both parts of the solution (that is benchmarked)
    virtual output_t run_solution(str_view input) const = 0;

    // Runs preliminary tests (optional, excluded from benchmark)
    virtual void run_tests() const = 0;
};

// Factory container for all Advent of Code problems arranged by year / day.
// Use @see ADVENT_DAY() macro to automatically register a new day.
// Ref: https://www.cppstories.com/2018/02/factory-selfregister/
class DayFactory
{
  public:
    using create_fn_t = std::unique_ptr<IDay>(*)();
    using year_map_t = std::map<size_t, create_fn_t>;

  public:
    DayFactory() = delete;

    static bool add_day(size_t year, size_t day, const create_fn_t& fn);

    static std::unique_ptr<IDay> create(size_t year, size_t day);

    static std::map<size_t, year_map_t>& get_years() { return s_years; }

  private:
    static std::map<size_t, year_map_t> s_years;
};


// Expands to the name of the class that implements the given test.
// Ref: GTEST_TEST_CLASS_NAME_ (gtest-internal.h)
#define DAY_NAME_(year, day) \
  Day_##year##_##day

// Helper macro for defining new days.
// Ref: https://www.cppstories.com/2018/02/factory-selfregister/
#define ADVENT_DAY(year, day, answer_pt1, answer_pt2)   \
class DAY_NAME_(year, day) : public IDay { \
public: \
    static std::unique_ptr<IDay> create() { return std::make_unique<DAY_NAME_(year, day)>(); } \
    void run_tests() const override; \
    output_t run_solution(str_view input) const override; \
    output_t get_valid() const override { return { answer_pt1, answer_pt2 }; } \
private: \
    static bool s_registered; \
}; \
bool DAY_NAME_(year, day)::s_registered = DayFactory::add_day(year, day, DAY_NAME_(year, day)::create);

