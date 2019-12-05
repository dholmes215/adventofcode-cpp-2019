// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include "intcode.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

using namespace intcode;
using namespace std;

int main(int /*argc*/, char ** /*argv*/)
{
    const Memory input_memory = read_input(cin);

    // Part 1
    {
        Memory memory = input_memory;
        memory[1] = 12;
        memory[2] = 2;

        Computer computer(move(memory));
        computer.run();

        cout << "Part 1: address 0 is " << computer.memory[0] << '\n';
    }

    // Part 2
    for (int32_t noun = 0; noun < 100; noun++) {
        for (int32_t verb = 0; verb < 100; verb++) {
            Memory memory = input_memory;
            memory[1] = noun;
            memory[2] = verb;
            Computer computer(move(memory));
            computer.run();
            if (computer.memory[0] == 19690720) {
                cout << "Part 2: answer is " << (noun * 100 + verb)
                     << " (noun=" << noun << " verb=" << verb << ")\n";
                return 0;
            }
        }
    }

    return 0;
}
