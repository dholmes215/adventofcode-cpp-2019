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

stringstream run_computer(const Memory & memory, stringstream & in)
{
    Computer computer(memory);
    stringstream out;
    computer.run(in, out);
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

int32_t run_amplifier(const Memory & memory, const int32_t phase_setting,
                      const int32_t input)
{
    stringstream in_stream;
    in_stream << phase_setting << '\n' << input << '\n';
    auto out_stream = run_computer(memory, in_stream);
    int32_t out;
    out_stream >> out;
    assert(out_stream);
    return out;
}

int32_t run_all_amplifiers(const Memory & memory,
                           const phase_settings & settings, const int32_t input)
{
    auto out = run_amplifier(memory, settings.a, input);
    out = run_amplifier(memory, settings.b, out);
    out = run_amplifier(memory, settings.c, out);
    out = run_amplifier(memory, settings.d, out);
    out = run_amplifier(memory, settings.e, out);
    return out;
}

int main(int argc, char ** argv)
{
    ifstream input_file(get_filename(argc, argv));

    const Memory input_memory = read_input(input_file);

    // Part 1
    {
        phase_settings highest_settings = {0,1,2,3,4};
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



    return 0;
}
