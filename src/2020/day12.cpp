#include "setup/all.h"

ADVENT_DAY(2020, 12, 364, 39518);

namespace
{
    enum action_t : uint8_t
    {
        MOV_N,
        MOV_S,
        MOV_E,
        MOV_W,
        ROT_L,
        ROT_R,
        MOV_F,
        ERR
    };

    enum dir_t : uint8_t
    {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    struct ship_pos_t
    {
        vec2i pos { 0, 0 };
        dir_t dir { dir_t::EAST };
    };

    struct order_t
    {
        action_t action;
        uint value;
    };

    [[maybe_unused]]
    void print(const ship_pos_t& ship)
    {
        char dir = '?';

        switch (ship.dir)
        {
            case NORTH: { dir = 'N'; break; }
            case EAST:  { dir = 'E'; break; }
            case SOUTH: { dir = 'S'; break; }
            case WEST:  { dir = 'W'; break; }
        }

        printf("  pos: %d, %d, \tdir: %c\n", ship.pos.x, ship.pos.y, dir);
    }

    [[maybe_unused]]
    void print(const order_t& order, const ship_pos_t& pos)
    {
        char ch = '?';
        switch(order.action)
        {
            case MOV_N: { ch = 'N'; break; }
            case MOV_S: { ch = 'S'; break; }
            case MOV_E: { ch = 'E'; break; }
            case MOV_W: { ch = 'W'; break; }
            case ROT_L: { ch = 'L'; break; }
            case ROT_R: { ch = 'R'; break; }
            case MOV_F: { ch = 'F'; break; }
            case ERR: { ch = '!'; break; }
        }

        printf("action: %c %d, \t", ch, order.value);
        print(pos);
    }

    action_t char_to_action(const char ch)
    {
        switch (ch)
        {
            case 'N': return MOV_N;
            case 'S': return MOV_S;
            case 'E': return MOV_E;
            case 'W': return MOV_W;
            case 'L': return ROT_L;
            case 'R': return ROT_R;
            case 'F': return MOV_F;
        }
        return ERR;
    }

    std::vector<order_t> parse(str_view input)
    {
        std::vector<order_t> orders;
        orders.reserve(1000);

        parse_input(input, [&](str_view line)
        {
            orders.push_back(order_t {
                .action = char_to_action(line[0]),
                .value = to_int<uint16_t>(str_view { &line[1], line.size() - 1 })
            });
        });

        return orders;
    }

    ship_pos_t iterate_orders_pt1(const std::vector<order_t>& orders)
    {
        ship_pos_t ship;

        auto rotate = [](const dir_t dir, int sign, uint value) -> dir_t
        {
            return static_cast<dir_t>((dir + sign * (value / 90)) % 4);
        };

        for (const auto& [action, value] : orders)
        {
            switch (action)
            {
                case MOV_N: { ship.pos.y += value; break; }
                case MOV_S: { ship.pos.y -= value; break; }
                case MOV_E: { ship.pos.x += value; break; }
                case MOV_W: { ship.pos.x -= value; break; }
                case ROT_L: { ship.dir = rotate(ship.dir, -1, value); break; }
                case ROT_R: { ship.dir = rotate(ship.dir, +1, value); break; }
                case MOV_F:
                {
                    switch (ship.dir)
                    {
                        case NORTH: { ship.pos.y += value; break; }
                        case EAST:  { ship.pos.x += value; break; }
                        case SOUTH: { ship.pos.y -= value; break; }
                        case WEST:  { ship.pos.x -= value; break; }
                    }
                    break;
                }
                case ERR: { assert(false); break; }
            }
        }

        return ship;
    }

    vec2i iterate_orders_pt2(const std::vector<order_t>& orders)
    {
        vec2i ship { 0, 0 };
        vec2i target { 10, 1 };

        auto rotate = [](const vec2i& dir, int sign, uint value) -> vec2i
        {
            if (value == 90) return sign * vec2i{ dir.y, -dir.x };
            if (value == 180) return { -dir.x, -dir.y };
            if (value == 270) return sign * vec2i{ -dir.y, dir.x };
            else return dir;
        };

        for (const auto& [action, value]: orders)
        {
            switch (action)
            {
                case MOV_N: { target.y += value; break; }
                case MOV_S: { target.y -= value; break; }
                case MOV_E: { target.x += value; break; }
                case MOV_W: { target.x -= value; break; }
                case ROT_L: { target = rotate(target, -1, value); break; }
                case ROT_R: { target = rotate(target, +1, value); break; }
                case MOV_F: { ship += target * value; break; }
                case ERR: { assert(false); break; }
            }
        }
        return ship;
    }
}

result_t Day_2020_12::run_solution(str_view input) const
{
    std::vector<order_t> orders = parse(input);

    ship_pos_t ship1 = iterate_orders_pt1(orders);
    vec2i ship2 = iterate_orders_pt2(orders);

    return {
        static_cast<size_t>(abs(ship1.pos.x) + abs(ship1.pos.y)),
        static_cast<size_t>(abs(ship2.x) + abs(ship2.y))
    };
}

void Day_2020_12::run_tests() const
{
    char text1[] = "F10\nN3\nF7\nR90\nF11\n";

    std::vector<order_t> orders = parse(text1);
    assert(orders.size() != 0);

    ship_pos_t ship = iterate_orders_pt1(orders);
    assert(ship.pos.x == 17);
    assert(ship.pos.y == -8);
    assert(ship.dir == dir_t::SOUTH);
    assert(abs(ship.pos.x) + abs(ship.pos.y) == 25);

    vec2i ship2 = iterate_orders_pt2(orders);
    assert(ship2.x == 214);
    assert(ship2.y == -72);
    assert(abs(ship2.x) + abs(ship2.y) == 286);
}
