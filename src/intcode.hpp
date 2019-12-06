// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#ifndef AOC_INTCODE_HPP
#define AOC_INTCODE_HPP

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

namespace intcode {

using Memory = std::vector<int32_t>;

Memory read_input(std::istream & input);
void print_memory(const Memory & memory);

class Computer
{
public:
    enum class State
    {
        Run,
        Halt,
    };

    Computer(const Memory & memory) : memory(memory) {}
    Computer(Memory && memory) : memory(move(memory)) {}
    Computer(std::istream & input) : memory(read_input(input)) {}

    State run_op(std::istream & in, std::ostream & out);
    State run_op();

    void run(std::istream & in, std::ostream & out)
    {
        while (run_op(in, out) == State::Run) {
            // Keep running...
        }
    }

    void run()
    {
        while (run_op() == State::Run) {
            // Keep running...
        }
    }

    Memory memory;
    int pc = 0;

private:
};

} // namespace intcode

#endif // AOC_INTCODE_HPP
