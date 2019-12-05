
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

    State run_op()
    {
        int32_t opcode = memory[pc];
        switch (opcode) {
        case 1: {
            int32_t operand1 = memory[memory[pc + 1]];
            int32_t operand2 = memory[memory[pc + 2]];
            memory[memory[pc + 3]] = operand1 + operand2;
            pc += 4;
            return State::Run;
        }
        case 2: {
            int32_t operand1 = memory[memory[pc + 1]];
            int32_t operand2 = memory[memory[pc + 2]];
            memory[memory[pc + 3]] = operand1 * operand2;
            pc += 4;
            return State::Run;
        }
        case 99:
        default:
            return State::Halt;
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
