#include "factory.h"

std::map<size_t, DayFactory::year_map_t> DayFactory::s_years;

bool DayFactory::add_day(size_t year, size_t day, const create_fn_t& fn)
{
    auto year_it = s_years.find(year);
    if (year_it == s_years.end())
    {
        s_years[year] = {};
    }

    auto day_it = s_years[year].find(day);
    if (day_it == s_years[year].end())
    {
        s_years[year][day] = fn;
        return true;
    }

    return false;
}

std::unique_ptr<IDay> DayFactory::create(size_t year, size_t day)
{
    auto year_it = s_years.find(year);
    if (year_it != s_years.end())
    {
        auto day_it = year_it->second.find(day);
        if (day_it != year_it->second.end())
        {
            return day_it->second(); // call the createFunc
        }
    }

    return nullptr;
}
