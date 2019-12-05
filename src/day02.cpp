// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using Memory = vector<int32_t>;

Memory read_input(istream & input)
{
    Memory out;
    int32_t num;
    input >> num;
    out.push_back(num);
    char comma;
    while (cin >> comma >> num) {
        out.push_back(num);
    }
    return out;
}

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
    Computer(istream & input) : memory(read_input(input)) {}

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

void print_memory(const Memory & memory)
{
    cout << setw(8) << memory[0];
    for (size_t i = 1; i < memory.size(); i++) {
        cout << setw(0) << ',';
        if (i % 4 == 0) {
            cout << setw(0) << '\n';
        }
        cout << setw(8) << memory[i];
    }
    cout << setw(0) << '\n';
}

int main(int /*argc*/, char ** /*argv*/)
{
    const Memory input_memory = read_input(cin);

    // Part 1
    {
        Memory memory = input_memory;
        memory[1] = 12;
        memory[2] = 2;

        Computer computer(move(memory));

        print_memory(computer.memory);

        computer.run();

        print_memory(computer.memory);

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
