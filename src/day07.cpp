// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include "intcode.hpp"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace intcode;
using namespace std;

const char * get_filename(int argc, char ** argv)
{
    if (argc < 2) {
        cerr << "USAGE: " << argv[0] << " filename.txt";
        exit(-1);
    }
    return argv[1];
}

vector<string> istream_to_vector(istream & stream)
{
    vector<string> out;
    string line;
    while (getline(stream, line)) {
        out.emplace_back(move(line));
    }
    return out;
}

struct phase_settings
{
    int a;
    int b;
    int c;
    int d;
    int e;
};

ostream & operator<<(ostream & s, const phase_settings & p)
{
    s << p.a << p.b << p.c << p.d << p.e;
    return s;
}

template <typename Integer> bool is_even(Integer k) { return k % 2 == 0; }

// Heap's algorithm (recursive) adapted from
// https://en.wikipedia.org/wiki/Heap%27s_algorithm
// TODO: could be optimized a lot
template <typename T>
void generate_permutations(int k, vector<T> & a, vector<vector<T>> & outputs)
{
    if (k == 1) {
        outputs.push_back(a);
        return;
    }

    // Generate permutations with kth unaltered
    // Initially k == length(A)
    generate_permutations(k - 1, a, outputs);

    // Generate permutations for kth swapped with each k-1 initial
    for (int i = 0; i < k - 1; i++) {
        // Swap choice dependent on parity of k (even or odd)
        if (is_even(k)) {
            swap(a[i], a[k - 1]);
        } else {
            swap(a[0], a[k - 1]);
        }
        generate_permutations(k - 1, a, outputs);
    }
}

const array<phase_settings, 120> phase_setting_permutations = [] {
    vector<int> inputs = {0, 1, 2, 3, 4};
    vector<vector<int>> permutations;
    generate_permutations(5, inputs, permutations);
    assert(permutations.size() == 120);
    array<phase_settings, 120> out;
    for (int i = 0; i < 120; i++) {
        out[i].a = permutations[i][0];
        out[i].b = permutations[i][1];
        out[i].c = permutations[i][2];
        out[i].d = permutations[i][3];
        out[i].e = permutations[i][4];
    }
    return out;
}();

const array<phase_settings, 120> feedback_loop_permutations = [] {
    vector<int> inputs = {5, 6, 7, 8, 9};
    vector<vector<int>> permutations;
    generate_permutations(5, inputs, permutations);
    assert(permutations.size() == 120);
    array<phase_settings, 120> out;
    for (int i = 0; i < 120; i++) {
        out[i].a = permutations[i][0];
        out[i].b = permutations[i][1];
        out[i].c = permutations[i][2];
        out[i].d = permutations[i][3];
        out[i].e = permutations[i][4];
    }
    return out;
}();

IntQueue run_computer(const Memory & memory, IntQueue & in)
{
    Computer computer(memory);
    IntQueue out;
    computer.run(in, out);

    return out;
}

Int run_amplifier(const Memory & memory, const Int phase_setting,
                  const Int input)
{
    IntQueue in;
    in.push(phase_setting);
    in.push(input);
    auto out = run_computer(memory, in);
    auto ret = out.front();
    out.pop();
    return ret;
}

int32_t run_all_amplifiers(const Memory & memory,
                           const phase_settings & settings, const Int input)
{
    auto out = run_amplifier(memory, settings.a, input);
    out = run_amplifier(memory, settings.b, out);
    out = run_amplifier(memory, settings.c, out);
    out = run_amplifier(memory, settings.d, out);
    out = run_amplifier(memory, settings.e, out);
    return out;
}

int32_t run_all_amplifiers_until_halted(const Memory & memory,
                                        const phase_settings & settings,
                                        const Int input)
{
    array<Computer, 5> computers = {memory, memory, memory, memory, memory};
    array<IntQueue, 5> io;

    // Write phase settings to all computers. All queues should be empty and all
    // computers should be waiting for input.
    io[0].push(settings.a);
    io[1].push(settings.b);
    io[2].push(settings.c);
    io[3].push(settings.d);
    io[4].push(settings.e);

    computers[0].run(io[0], io[1]);
    computers[1].run(io[1], io[2]);
    computers[2].run(io[2], io[3]);
    computers[3].run(io[3], io[4]);
    computers[4].run(io[4], io[0]);

    io[0].push(input);

    bool halted = false;
    while (!halted) {
        for (int i = 0; i < 5; i++) {
            IntQueue & in = io[i];
            IntQueue & out = io[(i + 1) % 5];
            auto state = computers[i].run(in, out);
            if (state == Computer::State::Halted) {
                halted = true;
            }
            assert(out.size() < 2);
        }
    }

    return io[0].front();
}

int main(int argc, char ** argv)
{
    ifstream input_file(get_filename(argc, argv));

    const Memory input_memory = read_input(input_file);

    // Part 1
    {
        phase_settings highest_settings = {0, 1, 2, 3, 4};
        int32_t highest_output = 0;
        for (const auto & settings : phase_setting_permutations) {
            auto out = run_all_amplifiers(input_memory, settings, 0);
            if (out > highest_output) {
                highest_output = out;
                highest_settings = settings;
            }
        }
        cout << "Part 1: phase settings " << highest_settings
             << " have highest signal: " << highest_output << '\n';
    }

    // Part 2
    {
        phase_settings highest_settings = {0, 1, 2, 3, 4};
        int32_t highest_output = 0;
        for (const auto & settings : feedback_loop_permutations) {
            auto out =
                run_all_amplifiers_until_halted(input_memory, settings, 0);
            if (out > highest_output) {
                highest_output = out;
                highest_settings = settings;
            }
        }
        cout << "Part 2: phase settings " << highest_settings
             << " have highest signal: " << highest_output << '\n';
    }

    return 0;
}
