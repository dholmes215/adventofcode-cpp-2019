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

Computer::State Computer::run_op(IntQueue & in, IntQueue & out)
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
        return State::Running;
    }
    case 2: { // Multiplication
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        memory[memory[pc + 3]] = operand1 * operand2; // Never immediate mode
        pc += 4;
        return State::Running;
    }
    case 3: { // Input
        if (in.empty()) {
            return State::Waiting;
        }
        int32_t input = in.front();
        in.pop();
        memory[memory[pc + 1]] = input;
        pc += 2;
        return State::Running;
    }
    case 4: { // Output
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        out.push(operand1);
        pc += 2;
        return State::Running;
    }
    case 5: { // jump-if-true
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        if (operand1) {
            pc = operand2;
        } else {
            pc += 3;
        }
        return State::Running;
    }
    case 6: { // jump-if-false
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        if (!operand1) {
            pc = operand2;
        } else {
            pc += 3;
        }
        return State::Running;
    }
    case 7: { // less than
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        memory[memory[pc + 3]] = (operand1 < operand2); // Never immediate mode
        pc += 4;
        return State::Running;
    }
    case 8: { // equals
        int32_t operand1 = mode1 ? memory[pc + 1] : memory[memory[pc + 1]];
        int32_t operand2 = mode2 ? memory[pc + 2] : memory[memory[pc + 2]];
        memory[memory[pc + 3]] = (operand1 == operand2); // Never immediate mode
        pc += 4;
        return State::Running;
    }
    case 99:
    default:
        return State::Halted;
    }
}

} // namespace intcode
