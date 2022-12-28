#include "setup/all.h"

ADVENT_DAY(2022, 16, 1474, 2100);

namespace 
{
    struct graph_t 
    {
        uint32_t n_nodes = 0; // number of nodes
        std::vector<str_view> names; // nodes names
        std::vector<int32_t> flows; // node values
        std::vector<int32_t> costs; // (n*n) matrix of transition costs

        graph_t(str_view input)
        {
            std::vector<std::array<str_view, 2>> parsed_data;
            for_each_split(input, '\n', [&](str_view line)
            {
                constexpr size_t node_idx = sizeof("Valve ") - 1;
                constexpr size_t rate_idx = sizeof("Valve ?? has flow rate=") - 1;
                constexpr size_t skip_idx = sizeof("tunnel leads to valve ") - 1;

                str_view node { &line[node_idx], 2 };
                auto [rate, end] = to_int<int32_t>({ &line[rate_idx], 3 }, 0);
                size_t tunnel_idx = rate_idx + end + 2;
                size_t mult_idx = (line[tunnel_idx+6] == 's') ? 1 : 0;
                size_t link_idx = tunnel_idx + skip_idx + mult_idx;
                str_view links_str { &line[link_idx], line.size() - link_idx };

                names.push_back(node);
                flows.push_back(rate);
                n_nodes++;
                parsed_data.push_back({ node, links_str });
            });

            update_costs(parsed_data);
            simplify_table();
        }

        uint32_t get_idx(str_view node) const
        {
            for (uint32_t i = 0; i < n_nodes; i++)
                if (names[i] == node)
                    return i;
            return UINT32_MAX;
        }

        [[maybe_unused]]
        void print() const 
        {
            fmt::print("graph (n={}): \n", n_nodes);
            fmt::print(" - names: {}\n", names);
            fmt::print(" - flows: {}\n", flows);
            fmt::print(" - costs:\n");

            for (size_t y = 0; y < n_nodes; y++)
            {
                fmt::print("   ");
                for (size_t x = 0; x < n_nodes; x++)
                {
                    auto cost = costs[y * n_nodes + x];
                    if (cost == INT32_MAX)
                        fmt::print(COLOR_GRAY() "inf " COLOR_RESET());
                    else 
                        fmt::print("{:3} ", costs[y * n_nodes + x]);
                }
                fmt::print("\n");
            }
        }

        size_t get_max_pressure(std::array<int32_t, 2> timers) const
        {
            struct state_t {
                uint32_t valves; // opened valve indexes
                int32_t pressure; // total
                std::array<uint32_t, 2> nodes;
                std::array<int32_t, 2> timers;

                // Following heuristic gets score's upper bound, by opening
                // all unopened valves at the next possible step and summing
                // their effects. The players cannot move like this, so the 
                // bound is unreachable, but it allows us to prune away paths 
                // that _definitely_ can't win the current best score. 
                int32_t upper_bound(const graph_t& graph) const 
                {
                    int32_t upper_bound = pressure;
                    for (size_t target = 0; target < graph.n_nodes; target++)
                    {
                        if (get_bit(valves, target))
                            continue;

                        for (size_t agent = 0; agent < 2; agent++)
                        {
                            int32_t cost = graph.costs[nodes[agent] * graph.n_nodes + target] + 1;
                            if (cost <= timers[agent])
                            {
                                upper_bound += (graph.flows[target] * (timers[agent] - cost));
                            }
                        }
                    }

                    return upper_bound;
                }
            };

            int32_t max_pressure = 0;
            std::vector<state_t> queue { state_t { 
                .valves = 0, 
                .pressure = 0, 
                .nodes = { get_idx("AA"), get_idx("AA") },
                .timers = timers,
            }};

            while (!queue.empty())
            {
                const state_t curr = queue.back();
                queue.pop_back();

                if (curr.upper_bound(*this) < max_pressure)
                    continue;
                
                for (uint32_t i = 0; i < n_nodes; i++)
                {
                    // The next player to move is the one with most time.
                    size_t idx = (curr.timers[0] > curr.timers[1]) ? 0 : 1;
                    int32_t cost = costs[curr.nodes[idx] * n_nodes + i] + 1;
                    if (get_bit(curr.valves, i) || curr.timers[idx] <= cost)
                        continue;

                    state_t next = curr;
                    set_bit(next.valves, i);
                    next.timers[idx] -= cost;
                    next.nodes[idx] = i;
                    next.pressure += (flows[i] * next.timers[idx]);
                    max_pressure = max(max_pressure, next.pressure);

                    if (next.upper_bound(*this) > max_pressure)
                    {
                        queue.push_back(next);
                    }
                }
            }

            return max_pressure;
        }
    
        private:
            // Update transition cost table
            void update_costs(const std::vector<std::array<str_view, 2>>& parsed_data)
            {
                // Initialize cost table
                costs = std::vector<int32_t>(n_nodes * n_nodes);
                for (auto& cost: costs) 
                    cost = INT32_MAX;

                // Add immediate links (with cost 1)
                for (auto [node, links_str]: parsed_data)
                    for (auto link: split(links_str, ", "))
                        costs[get_idx(node) * n_nodes + get_idx(link)] = 1;

                // Floyd-Warshall to fill in shortest paths between all possible node pairs
                const size_t n = n_nodes;
                for (size_t k = 0; k < n; k++)
                    for (size_t i = 0; i < n; i++)
                        for (size_t j = 0; j < n; j++)
                            costs[i+j*n] = min(costs[i+j*n], sat_add(costs[i+k*n], costs[k+j*n]));

                // Update diagonal back to `inf`, instead of each node being "2 nodes away" from itself
                for (size_t i = 0; i < n; i++)
                    costs[i+i*n] = INT32_MAX;
            }

            // Removes irrelevant nodes (with flow rate = 0)
            void simplify_table()
            {
                std::vector<str_view> new_names;
                std::vector<int32_t> new_flows;
                std::vector<uint32_t> preserve_idxs;
                for (uint32_t i = 0; i < flows.size(); i++)
                {
                    if (flows[i] > 0 || names[i] == "AA")
                    {
                        new_names.push_back(names[i]);
                        new_flows.push_back(flows[i]);
                        preserve_idxs.push_back(i);
                    }
                }

                const uint32_t new_n = (uint32_t)preserve_idxs.size();
                std::vector<int32_t> new_costs(new_n * new_n);
                for (uint32_t i = 0; i < new_n; i++)
                {
                    for (uint32_t j = 0; j < new_n; j++)
                    {
                        size_t old_i = preserve_idxs[i];
                        size_t old_j = preserve_idxs[j];
                        new_costs[i * new_n + j] = costs[old_i * n_nodes + old_j];
                    }
                }

                n_nodes = new_n;
                costs = new_costs;
                names = new_names;
                flows = new_flows;
            }
    };
}

result_t Day_2022_16::run_solution(str_view input) const
{
    graph_t graph(input);
    // graph.print();

    size_t part1 = graph.get_max_pressure({ 30, 0 });
    size_t part2 = graph.get_max_pressure({ 26, 26 });

    return { part1, part2 };
}

void Day_2022_16::run_tests() const
{
    std::string test_input = 
        "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB\n"
        "Valve BB has flow rate=13; tunnels lead to valves CC, AA\n"
        "Valve CC has flow rate=2; tunnels lead to valves DD, BB\n"
        "Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE\n"
        "Valve EE has flow rate=3; tunnels lead to valves FF, DD\n"
        "Valve FF has flow rate=0; tunnels lead to valves EE, GG\n"
        "Valve GG has flow rate=0; tunnels lead to valves FF, HH\n"
        "Valve HH has flow rate=22; tunnel leads to valve GG\n"
        "Valve II has flow rate=0; tunnels lead to valves AA, JJ\n"
        "Valve JJ has flow rate=21; tunnel leads to valve II\n";

    graph_t graph(test_input);
    // graph.print();

    size_t test_part1 = graph.get_max_pressure({ 30, 0 });
    assert(test_part1 == 1651);

    size_t test_part2 = graph.get_max_pressure({ 26, 26 });
    assert(test_part2 == 1707);
}
