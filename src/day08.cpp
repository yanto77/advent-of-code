#include "advent2020.h"
#include <iostream>

enum op_type_t : uint8_t
{ 
    ERR, // error, default value
    ACC, // accumulate `arg1`
    JMP, // translate instruction pointer by `arg1`
    NOP, // no operation
    RET  // return from execution
};

struct op_t
{
    op_type_t type = op_type_t::ERR;
    int16_t arg1 = 0;
};

struct registers_t
{
    size_t ip = 0; // instruction pointer
    int acc = 0; // accumulator
};

typedef std::vector<op_t> program_t;

namespace
{
    [[maybe_unused]]
    void print(const program_t& prg, const registers_t& regs)
    {
        printf(" - Program size: %zu. Registers: ip %zu, acc: %d\n", prg.size(), regs.ip, regs.acc);
    }

    program_t parse_input(const input_t& input)
    {
        program_t program;
        program.reserve(1000);

        parse_input(input, [&](const sv& line) 
        {
            op_t op;
            if (line.starts_with("acc")) { op.type = op_type_t::ACC; }
            else if (line.starts_with("jmp")) { op.type = op_type_t::JMP; }
            else if (line.starts_with("nop")) { op.type = op_type_t::NOP; }
            else { assert(false && "not implemented"); }

            sv value_str { &line[5], (line.size() - 5) };
            const int16_t sign = (line[4] == '+') ? 1 : -1;
            const int16_t value = static_cast<int16_t>(to_int(value_str));
            op.arg1 = sign * value;

            program.push_back(op);
        });

        return program;
    }

    registers_t execute_until_loop(program_t& program)
    {
        registers_t reg;
        const size_t last_instr = (program.size() - 1);

        while (true)
        {
            if (reg.ip == last_instr)
            {
                return reg;
            }

            op_t& op = program[reg.ip];
            switch (op.type)
            {
                case op_type_t::NOP: {                        ++reg.ip; break; }
                case op_type_t::ACC: { reg.acc += op.arg1;    ++reg.ip; break; }
                case op_type_t::JMP: { reg.ip += op.arg1;               break; }
                case op_type_t::ERR: { assert(false);                   break; }
                case op_type_t::RET: { return reg; }
            }

            op.type = op_type_t::RET;
        }
    }

    registers_t bruteforce_find(const program_t& prg)
    {
        for (size_t i = 0; i < prg.size(); ++i)
        {
            program_t copy = prg;
            if (copy[i].type == op_type_t::NOP) 
            { 
                copy[i].type = op_type_t::JMP; 
            }
            else if (copy[i].type == op_type_t::JMP) 
            { 
                copy[i].type = op_type_t::NOP; 
            }
            else 
            {
                continue; 
            }

            const registers_t& regs = execute_until_loop(copy);
            if (regs.ip == (prg.size() - 1))
            {
                return regs;
            }
        }

        return {};
    }
}

void day08(const input_t& input)
{
    const program_t& prg = parse_input(input);
    
    // Part 1
    {
        program_t copy = prg;
        const registers_t& reg = execute_until_loop(copy);
        assert(reg.ip == 346 && reg.acc == 1949);
    }

    // Part 2
    {
        const registers_t& reg = bruteforce_find(prg);
        assert(reg.ip == 646 && reg.acc == 2092);
    }
}

void day08_test()
{
    char input1[65] = "nop +0\n"
        "acc +1\n"
        "jmp +4\n"
        "acc +3\n"
        "jmp -3\n"
        "acc -99\n"
        "acc +1\n"
        "jmp -4\n"
        "acc +6\n";
    input_t test1 { input1, 65 };

    program_t prg = parse_input(test1);
    const registers_t& reg = execute_until_loop(prg);
    assert(reg.ip == 1 && reg.acc == 5);
}
