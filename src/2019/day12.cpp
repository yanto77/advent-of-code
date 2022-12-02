#include "setup/all.h"

ADVENT_DAY(2019, 12, 7722, 292653556339368);

namespace
{
    struct system_t
    {
        std::array<vec3i, 4> pos;
        std::array<vec3i, 4> vel;

        size_t compute_energy() const
        {
            size_t total = 0;
            for (size_t i = 0; i < pos.size(); i++)
            {
                size_t pot = pos[i].sum_abs();
                size_t kin = vel[i].sum_abs();
                total += (kin * pot);
            }
            return total;
        }

        void simulate(size_t timesteps)
        {
            for (size_t step = 0; step < timesteps; step++)
            {
                for (size_t e = 0; e < 3; e++)
                {
                    compute_step(e);
                }
            }
        }

        size_t compute_common_period()
        {
            size_t steps[3] = {0, 0, 0}; // for each dim

            auto get_state = [this](size_t dim) -> std::array<int, 8>
            {
                return {
                    pos[0][dim], pos[1][dim], pos[2][dim], pos[3][dim],
                    vel[0][dim], vel[1][dim], vel[2][dim], vel[3][dim]
                };
            };

            // compute periods per dimension
            for (size_t dim = 0; dim < 3; dim++)
            {
                const auto& copy = get_state(dim);
                do
                {
                    compute_step(dim);
                    steps[dim]++;
                }
                while (get_state(dim) != copy);
            }
            
            // combine periods into common period
            size_t res = lcm(lcm(steps[0], steps[1]), steps[2]);
            return res;
        }

      private:
        void compute_step(size_t dim)
        {
            // update velocity by applying gravity
            for (size_t i1 = 0; i1 < pos.size(); i1++)
            {
                for (size_t i2 = i1 + 1; i2 < pos.size(); i2++)
                {
                    int delta = pos[i1][dim] - pos[i2][dim];
                    if (delta != 0)
                    {
                        delta = delta > 0 ? 1 : -1;
                        vel[i1][dim] -= delta;
                        vel[i2][dim] += delta;
                    }
                }
            }

            // update positions by applying velocity
            for (size_t i = 0; i < pos.size(); i++)
            {
                pos[i][dim] += vel[i][dim];
            }
        }
    };
}

result_t Day_2019_12::run_solution(str_view /* input */) const
{
    system_t sys = { 
        .pos = { 
            vec3i{-4, 3, 15}, 
            vec3i{-11, -10, 13}, 
            vec3i{2, 2, 18}, 
            vec3i{7, -1, 0},
        }
    };

    sys.simulate(1000);
    size_t part1 = sys.compute_energy();
    size_t part2 = sys.compute_common_period();
    return {part1, part2};
}

void Day_2019_12::run_tests() const
{
    {
        system_t sys = {
            .pos = { 
                vec3i{-1, 0, 2},
                vec3i{2, -10, -7},
                vec3i{4, -8, 8},
                vec3i{3, 5, -1},
            }
        };

        sys.simulate(10);
        assert(179 == sys.compute_energy());
        assert(2772 == sys.compute_common_period());
    }

    {
        system_t sys = {
            .pos = { 
                vec3i{-8, -10, 0},
                vec3i{5, 5, 10},
                vec3i{2, -7, 3},
                vec3i{9, -8, -3},
            }
        };

        sys.simulate(100);
        assert(1940 == sys.compute_energy());
        assert(4686774924 == sys.compute_common_period());
    }
}
