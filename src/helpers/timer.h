#pragma once
#include <chrono>

class stack_timer_t
{
    typedef std::chrono::steady_clock clock_t;

    public:
        stack_timer_t(const char* _msg)
            : t0(clock_t::now()), msg(_msg)
        {
        }

        ~stack_timer_t()
        {
            auto elapsed = (clock_t::now() - t0);
            int64_t time = (elapsed.count() / 1000);
            printf("[timer] %s %d μs\n", msg, time);
        }

    private:
        std::chrono::time_point<clock_t> t0;
        const char* msg;
};
