#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <helpers/string.h>

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

class intcode_solver_io_t
{
  public:
    void set_input(const std::vector<int64_t>& data)
    {
        input_data = data;
    }

    void append_input(int64_t value)
    {
        input_data.push_back(value);
    }

    std::vector<int64_t> get_output()
    {
        return output_data;
    }

    void reset()
    {
        idp = 0;
        input_data.clear();
        output_data.clear();
    }

  private:
    friend class intcode_solver_t;
    int64_t get_next_input()
    {
        return input_data[idp++];
    }

    void append_output(int64_t value)
    {
        output_data.push_back(value);
    }

  public:
    size_t idp = 0; // input data pointer
    std::vector<int64_t> input_data;
    std::vector<int64_t> output_data;
};

class intcode_solver_t
{
  public:
    intcode_solver_t()
    {
        reset();
    }

    intcode_solver_t(const sv& input)
        : program(to_multi_int<int64_t>(input))
    {
        reset();
    }

    intcode_solver_t(const std::string& input)
        : intcode_solver_t(sv { input.data(), input.length() })
    {
        reset();
    }

    // execute whole program, until it halts, or outputs data and yields are accepted
    // returns the last opcode, when yielding/halting
    uint8_t execute(bool accept_yields = false)
    {
        while (!is_halted)
        {
            debug_pre_exec();

            uint8_t opcode = execute_once();

            debug_post_exec();

            if (accept_yields && (opcode == instr::OUTPUT))
            {
                return opcode;
            }
        }

        return instr::HALT;
    }

    void reset()
    {
        ip = 0;
        rbp = 0;
        is_halted = false;
        memory = program;
        io.reset();
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
                *get_addr(1, m1) = io.get_next_input();
                ip += 2;
                break;
            }
            case instr::OUTPUT:
            {
                io.append_output(*get_addr(1, m1));
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
    size_t ip = 0; // instruction pointer
    size_t rbp = 0; // relative base pointer

    bool is_halted = false;

    const std::vector<int64_t> program; // program, constant (always equal to input)
    std::vector<int64_t> memory; // program, as loaded into memory, may be modified

    intcode_solver_io_t io;
};
