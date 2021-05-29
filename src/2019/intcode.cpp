#include "intcode.h"
#include "setup/all.h"

namespace
{
    struct intcode_test
    {
        std::string input;
        std::vector<int32_t> end_state;
    };

    const std::vector<intcode_test> test_data =
    {
        intcode_test {
            .input = "1,0,0,0,99\n",
            .end_state = { 2, 0, 0, 0, 99 }
        },
        intcode_test {
            .input = "2,3,0,3,99\n",
            .end_state = { 2, 3, 0, 6, 99 }
        },
        intcode_test {
            .input = "2,4,4,5,99,0\n",
            .end_state = { 2, 4, 4, 5, 99, 9801 }
        },
        intcode_test {
            .input = "1,1,1,4,99,5,6,0,99\n",
            .end_state = { 30, 1, 1, 4, 2, 5, 6, 0, 99 }
        },
        intcode_test {
            .input = "1,9,10,3,2,3,11,0,99,30,40,50\n",
            .end_state = { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 }
        },
    };
}

void intcode_solver_t::run_tests()
{
    for (const auto& test: test_data)
    {
        intcode_solver_t solver(test.input);
        // printf("Input : "); print_row(data);

        solver.execute();
        // printf("Result: "); print_row(data);

        assert(solver.program == test.end_state);
    }
}
