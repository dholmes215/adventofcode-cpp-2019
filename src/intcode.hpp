// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#ifndef AOC_INTCODE_HPP
#define AOC_INTCODE_HPP

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

namespace intcode {

using Int = std::int32_t;
using Memory = std::vector<Int>;
using IntQueue = std::queue<Int>;

Memory read_input(std::istream & input);
void print_memory(const Memory & memory);

class Computer
{
public:
    enum class State
    {
        Running,
        Waiting,
        Halted,
    };

    Computer(const Memory & memory) : memory(memory) {}
    Computer(Memory && memory) : memory(move(memory)) {}
    Computer(std::istream & input) : memory(read_input(input)) {}

    State run_op(IntQueue & in, IntQueue & out);

    State run(IntQueue & in, IntQueue & out)
    {
        // Always run at least one instruction
        State state = State::Running;
        while (state == State::Running) {
            state = run_op(in, out);
        }
        return state;
    }

    State run()
    {
        IntQueue in;
        IntQueue out;
        Computer::State state = Computer::State::Running;
        while (state != Computer::State::Halted) {
            state = run(in, out);
            while (!out.empty()) {
                std::cout << out.front() << '\n';
                out.pop();
            }
            if (state == Computer::State::Waiting) {
                Int i;
                std::cin >> i;
                in.push(i);
            }
        }
        return state;
    }

    Memory memory;
    int pc = 0;

private:
};

} // namespace intcode

#endif // AOC_INTCODE_HPP
