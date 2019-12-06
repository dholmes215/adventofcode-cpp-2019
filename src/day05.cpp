// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include "intcode.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
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

int main(int argc, char ** argv)
{
    ifstream input_file(get_filename(argc, argv));

    const Memory input_memory = read_input(input_file);

    // Part 1
    {
        stringstream in("1\n");
        auto out = run_computer(input_memory, in);
        auto lines = istream_to_vector(out);
        cout << "Part 1: system 1 diagnostic code is " << lines.back() << '\n';
    }

    // Part 2
    {
        stringstream in("5\n");
        auto out = run_computer(input_memory, in);
        auto lines = istream_to_vector(out);
        cout << "Part 2: system 5 diagnostic code is " << lines.back() << '\n';
    }

    return 0;
}
