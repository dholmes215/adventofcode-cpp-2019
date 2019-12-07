// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include "intcode.hpp"

#include <fstream>
#include <iostream>

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

int main(int argc, char ** argv)
{
    ifstream input_file(get_filename(argc, argv));
    const Memory input_memory = read_input(input_file);
    Computer computer(input_memory);
    computer.run();
}