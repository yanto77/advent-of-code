#include "setup/all.h"

ADVENT_DAY(2019, 12, 7722, 0);

namespace
{
    struct planet_t 
    {
        vec3i pos;
        vec3i vel;
    };

    struct system_t
    {
        std::array<vec3i, 4> positions;
        std::array<vec3i, 4> velocities;

        friend bool operator==(const system_t& lhs, const system_t& rhs)
        { 
            return lhs.positions[0] == rhs.positions[0] 
                && lhs.positions[1] == rhs.positions[1] 
                && lhs.positions[2] == rhs.positions[2] 
                && lhs.positions[3] == rhs.positions[3]
                && lhs.velocities[0] == rhs.velocities[0] 
                && lhs.velocities[1] == rhs.velocities[1] 
                && lhs.velocities[2] == rhs.velocities[2] 
                && lhs.velocities[3] == rhs.velocities[3];
        }
        
        friend bool operator!=(const system_t& lhs, const system_t& rhs)
        { 
            return !(lhs == rhs); 
        }

        size_t compute_energy() const
        {
            size_t total = 0;
            for (size_t idx = 0; idx < positions.size(); idx++)
            {
                size_t pot = abs(positions[idx].x) + abs(positions[idx].y) + abs(positions[idx].z);
                size_t kin = abs(velocities[idx].x) + abs(velocities[idx].y) + abs(velocities[idx].z);

                total += (kin * pot);
                // fmt::print("pot: {}, kin: {}, total: {}\n", pot, kin, total);
            }
            return total;
        }

        void simulate_single_step()
        {
            // print state
            // fmt::print("after {} steps:\n", step);
            // for (size_t idx = 0; idx < positions.size(); idx++)
            // {
            //     fmt::print("pos=<x={:3d}, y={:3d}, z={:3d}>, vel=<x={:3d}, y={:3d}, z={:3d}>\n",
            //         positions[idx].x, positions[idx].y, positions[idx].z,
            //         velocities[idx].x, velocities[idx].y, velocities[idx].z);
            // }

            // update velocity by applying gravity
            for (size_t idx1 = 0; idx1 < positions.size(); idx1++)
            {
                for (size_t idx2 = idx1 + 1; idx2 < positions.size(); idx2++)
                {
                    int8_t delta_x = static_cast<int8_t>(positions[idx1].x > positions[idx2].x);
                    int8_t delta_y = static_cast<int8_t>(positions[idx1].y > positions[idx2].y);
                    int8_t delta_z = static_cast<int8_t>(positions[idx1].z > positions[idx2].z);

                    int8_t eq_x = 1 - static_cast<int8_t>(positions[idx1].x == positions[idx2].x);
                    int8_t eq_y = 1 - static_cast<int8_t>(positions[idx1].y == positions[idx2].y);
                    int8_t eq_z = 1 - static_cast<int8_t>(positions[idx1].z == positions[idx2].z);

                    velocities[idx1].x += eq_x * (1 - 2 * delta_x);
                    velocities[idx1].y += eq_y * (1 - 2 * delta_y);
                    velocities[idx1].z += eq_z * (1 - 2 * delta_z);

                    velocities[idx2].x += eq_x * (-1 + 2 * delta_x);
                    velocities[idx2].y += eq_y * (-1 + 2 * delta_y);
                    velocities[idx2].z += eq_z * (-1 + 2 * delta_z);
                }
            }

            // update positions by applying velocity
            for (size_t idx = 0; idx < positions.size(); idx++)
            {
                positions[idx] += velocities[idx];
            }
        }

        void simulate(size_t timesteps)
        {
            for (size_t step = 0; step < timesteps; step++)
            {
                simulate_single_step();
            }
        }

        size_t simulate_until_repeats()
        {
            size_t steps = 0;
            system_t copy = *this;

            do
            {
                simulate_single_step();
                steps++;

                if (steps % 100000000 == 0)
                    printf("steps: %zu\n", steps);
            }
            while (*this != copy);

            return steps;
        }
    };
}

output_t Day_2019_12::run_solution(const input_t& input) const
{
    (void)input;
    const system_t sys = {
        .positions = { 
            vec3i{-4, 3, 15}, 
            vec3i{-11, -10, 13}, 
            vec3i{2, 2, 18}, 
            vec3i{7, -1, 0},
        },
        .velocities = {}
    };

    auto sys1 = sys;
    sys1.simulate(1000);
    size_t part1 = sys1.compute_energy();

    auto sys2 = sys;
    size_t part2 = sys2.simulate_until_repeats();

    // 41.400.000.000, still not ready

    return {part1, part2};
}

void Day_2019_12::run_tests() const
{
    const system_t sys1 = {
        .positions = { 
            vec3i{ -1, 0, 2 },
            vec3i{ 2, -10, -7 },
            vec3i{ 4, -8, 8 },
            vec3i{ 3, 5, -1 },
        },
        .velocities = {}
    };

    const system_t sys2 = {
        .positions = { 
            vec3i{ -8, -10, 0 }, 
            vec3i{ 5, 5, 10 }, 
            vec3i{ 2, -7, 3 }, 
            vec3i{ 9, -8, -3 }, 
        },
        .velocities = {}
    };

    // part 1
    {
        auto sys = sys1;
        sys.simulate(10);
        assert(179 == sys.compute_energy());

        sys = sys2;
        sys.simulate(100);
        assert(1940 == sys.compute_energy());
    }

    // part 2
    {
        auto sys = sys1;
        assert(2772 == sys.simulate_until_repeats());

        // sys = sys2;
        // assert(4686774924 == sys.simulate_until_repeats());
    }
}
