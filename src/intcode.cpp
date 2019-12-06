// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include "intcode.hpp"

namespace intcode {

Memory read_input(std::istream & input)
{
    Memory out;
    int32_t num;
    input >> num;
    out.push_back(num);
    char comma;
    while (input >> comma >> num) {
        out.push_back(num);
    }
    return out;
}

void print_memory(const Memory & memory)
{
    std::cout << std::setw(8) << memory[0];
    for (size_t i = 1; i < memory.size(); i++) {
        std::cout << std::setw(0) << ',';
        if (i % 4 == 0) {
            std::cout << std::setw(0) << '\n';
        }
        std::cout << std::setw(8) << memory[i];
    }
    std::cout << std::setw(0) << '\n';
}

Computer::State Computer::run_op(std::istream & in, std::ostream & out)
{
    int32_t val = memory[pc];
    int32_t opcode = val % 100;
    int32_t mode1 = (val / 100) % 10;
    int32_t mode2 = (val / 1000) % 10;
    // int32_t mode3 = (val / 10000) % 10;
    switch (opcode) {
    case 1: { // Addition
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        memory[memory[pc + 3]] = operand1 + operand2; // Never immediate mode
        pc += 4;
        return State::Run;
    }
    case 2: { // Multiplication
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        memory[memory[pc + 3]] = operand1 * operand2; // Never immediate mode
        pc += 4;
        return State::Run;
    }
    case 3: { // Input
        int32_t input;
        in >> input;
        memory[memory[pc + 1]] = input;
        pc += 2;
        return State::Run;
    }
    case 4: { // Output
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        out << operand1 << '\n';
        pc += 2;
        return State::Run;
    }
    case 5: { // jump-if-true
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        if (operand1) {
            pc = operand2;
        } else {
            pc += 3;
        }
        return State::Run;
    }
    case 6: { // jump-if-false
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        if (!operand1) {
            pc = operand2;
        } else {
            pc += 3;
        }
        return State::Run;
    }
    case 7: { // less than
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        memory[memory[pc + 3]] = (operand1 < operand2); // Never immediate mode
        pc += 4;
        return State::Run;
    }
    case 8: { // equals
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        memory[memory[pc + 3]] = (operand1 == operand2); // Never immediate mode
        pc += 4;
        return State::Run;
    }
    case 99:
    default:
        return State::Halt;
    }
}

Computer::State Computer::run_op() { return run_op(std::cin, std::cout); }

} // namespace intcode
