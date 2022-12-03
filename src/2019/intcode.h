#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fmt/format.h>
#include <helpers/conv.h>
#include <helpers/string.h>
#include <helpers/num.h>
#include <vector>

enum instr: uint8_t
{
    SUM         = 1, // arg1 + arg2 => arg3
    MULTIPLY    = 2, // arg1 * arg2 => arg3
    INPUT       = 3, // write input to arg1
    OUTPUT      = 4, // output value of arg1

    JUMP_EQ     = 5,
    JUMP_NEQ    = 6,
    CMP_LESS    = 7,
    CMP_EQ      = 8,

    OFFSET_RBP  = 9, // update relative base pointer

    HALT        = 99
};

class intcode_solver_t
{
  public:
    intcode_solver_t()
    {
        reset();
    }

    intcode_solver_t(str_view input)
        : program(to_multi_int<int64_t>(input))
    {
        reset();
    }

    // execute whole program, until it halts, or, may yield on input/output instructions. 
    // NB: user is responsible for filling in the inputs immediately after yield!
    // returns the last opcode, when yielding/halting
    uint8_t execute(bool yield_on_io)
    {
        while (!is_halted)
        {
            // debug_pre_exec();

            uint8_t opcode = execute_once();

            // debug_post_exec();

            if (yield_on_io && (opcode == instr::OUTPUT || opcode == instr::INPUT))
            {
                return opcode;
            }
        }

        return instr::HALT;
    }

    // execute() where the input is predefined, so this runs through the program 
    // supplying the input values when asked for
    uint8_t execute(bool yield_on_io, const std::vector<int64_t>& input_data)
    {
        size_t input_idx = 0;
        while (!is_halted)
        {
            uint8_t op = execute(true);
            if (op == instr::HALT)
            {
                return op;
            }
            else if (op == instr::INPUT)
            {
                if (input_idx < input_data.size())
                    *idp = input_data[input_idx++];
                else return op;
            }
            else if (op == instr::OUTPUT && yield_on_io)
            {
                return op;
            }
        }

        return instr::HALT;
    }

    void reset()
    {
        ip = 0;
        rbp = 0;
        idp = nullptr;
        is_halted = false;
        memory = program;
        output_data.clear();
    }

    static void run_tests();

  private:
    // get pointer to memory address, guarantees the address exists
    // `n` -> position argument from instruction pointer
    // `immediate` -> parameter mode (true: value as is, false: memory addr)
    int64_t* get_addr(uint8_t n, uint8_t mode)
    {
        int64_t addr = -1;
        if (mode == 0)
        {
            addr = memory.at(ip + n);
        }
        else if (mode == 1)
        {
            addr = ip + n;
        }
        else if (mode == 2)
        {
            addr = rbp + memory.at(ip + n);
        }

        if (addr >= memory.size()) // ensure there's memory to read from
        {
            memory.resize(addr + 1, 0);
        }
        return &memory.at(addr);
    }

    // Get {opcode, mode of 1st param, mode of 2nd param, mode of 3rd param }
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> get_opcode() const
    {
        const auto& digits = get_digits<5>(memory[ip]);
        return { (digits[3] * 10 + digits[4]), digits[2], digits[1], digits[0] };
    }

    // execute single instruction, and return its opcode
    uint8_t execute_once()
    {
        const auto& [opcode, m1, m2, m3] = get_opcode();
        switch (opcode)
        {
            case instr::SUM:
            {
                *get_addr(3, m3) = *get_addr(1, m1) + *get_addr(2, m2);
                ip += 4;
                break;
            }
            case instr::MULTIPLY:
            {
                *get_addr(3, m3) = *get_addr(1, m1) * *get_addr(2, m2);
                ip += 4;
                break;
            }
            case instr::INPUT:
            {
                idp = get_addr(1, m1);
                ip += 2;
                break;
            }
            case instr::OUTPUT:
            {
                output_data.push_back(*get_addr(1, m1));
                ip += 2;
                break;
            }
            case instr::JUMP_EQ:
            {
                if (*get_addr(1, m1) != 0)
                    ip = *get_addr(2, m2);
                else
                    ip += 3;
                break;
            }
            case instr::JUMP_NEQ:
            {
                if (*get_addr(1, m1) == 0)
                    ip = *get_addr(2, m2);
                else
                    ip += 3;
                break;
            }
            case instr::CMP_LESS:
            {
                *get_addr(3, m3) = (*get_addr(1, m1) < *get_addr(2, m2)) ? 1 : 0;
                ip += 4;
                break;
            }
            case instr::CMP_EQ:
            {
                *get_addr(3, m3) = (*get_addr(1, m1) == *get_addr(2, m2)) ? 1 : 0;
                ip += 4;
                break;
            }
            case instr::OFFSET_RBP:
            {
                rbp += *get_addr(1, m1);
                ip += 2;
                break;
            }
            case instr::HALT:
            {
                is_halted = true;
                break;
            }
            default:
            {
                fmt::print("unsupported opcode: {}\n", opcode);
                assert(false);
                break;
            }
        }

        return opcode;
    }

    void debug_pre_exec();
    void debug_post_exec();

  public:
    bool is_halted = false;

    size_t ip = 0; // instruction pointer
    size_t rbp = 0; // relative base pointer
    int64_t* idp = nullptr; // input data pointer, valid immediately after program yields on INPUT, otherwise undefined

    const std::vector<int64_t> program; // program, constant (always equal to input)
    std::vector<int64_t> memory; // program, as loaded into memory, may be modified
    std::vector<int64_t> output_data;
};
