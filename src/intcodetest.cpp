// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include "intcode.hpp"

#include "gtest/gtest.h"

#include <sstream>
#include <string>

using namespace intcode;
using namespace std;

const char * day2_input_chars =
    "1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,6,1,19,1,5,19,23,1,13,23,27,1,6,27,31,2,"
    "31,13,35,1,9,35,39,2,39,13,43,1,43,10,47,1,47,13,51,2,13,51,55,1,55,9,59,"
    "1,59,5,63,1,6,63,67,1,13,67,71,2,71,10,75,1,6,75,79,1,79,10,83,1,5,83,87,"
    "2,10,87,91,1,6,91,95,1,9,95,99,1,99,9,103,2,103,10,107,1,5,107,111,1,9,"
    "111,115,2,13,115,119,1,119,10,123,1,123,10,127,2,127,10,131,1,5,131,135,1,"
    "10,135,139,1,139,2,143,1,6,143,0,99,2,14,0,0\n";

const char * day5_input_chars =
    "3,225,1,225,6,6,1100,1,238,225,104,0,1102,40,93,224,1001,224,-3720,224,4,"
    "224,102,8,223,223,101,3,224,224,1,224,223,223,1101,56,23,225,1102,64,78,"
    "225,1102,14,11,225,1101,84,27,225,1101,7,82,224,1001,224,-89,224,4,224,"
    "1002,223,8,223,1001,224,1,224,1,224,223,223,1,35,47,224,1001,224,-140,224,"
    "4,224,1002,223,8,223,101,5,224,224,1,224,223,223,1101,75,90,225,101,9,122,"
    "224,101,-72,224,224,4,224,1002,223,8,223,101,6,224,224,1,224,223,223,1102,"
    "36,63,225,1002,192,29,224,1001,224,-1218,224,4,224,1002,223,8,223,1001,"
    "224,7,224,1,223,224,223,102,31,218,224,101,-2046,224,224,4,224,102,8,223,"
    "223,101,4,224,224,1,224,223,223,1001,43,38,224,101,-52,224,224,4,224,1002,"
    "223,8,223,101,5,224,224,1,223,224,223,1102,33,42,225,2,95,40,224,101,-"
    "5850,224,224,4,224,1002,223,8,223,1001,224,7,224,1,224,223,223,1102,37,66,"
    "225,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,"
    "1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,"
    "265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,"
    "99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,"
    "99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,1007,226,677,224,"
    "1002,223,2,223,1005,224,329,1001,223,1,223,1007,226,226,224,1002,223,2,"
    "223,1006,224,344,101,1,223,223,1107,677,226,224,102,2,223,223,1006,224,"
    "359,1001,223,1,223,108,677,677,224,1002,223,2,223,1006,224,374,1001,223,1,"
    "223,107,677,677,224,1002,223,2,223,1005,224,389,101,1,223,223,8,677,677,"
    "224,1002,223,2,223,1005,224,404,1001,223,1,223,108,226,226,224,1002,223,2,"
    "223,1005,224,419,101,1,223,223,1008,677,677,224,1002,223,2,223,1005,224,"
    "434,101,1,223,223,1008,226,226,224,1002,223,2,223,1005,224,449,101,1,223,"
    "223,7,677,226,224,1002,223,2,223,1006,224,464,1001,223,1,223,7,226,226,"
    "224,1002,223,2,223,1005,224,479,1001,223,1,223,1007,677,677,224,102,2,223,"
    "223,1005,224,494,101,1,223,223,1108,677,226,224,102,2,223,223,1006,224,"
    "509,1001,223,1,223,8,677,226,224,102,2,223,223,1005,224,524,1001,223,1,"
    "223,1107,226,226,224,102,2,223,223,1006,224,539,1001,223,1,223,1008,226,"
    "677,224,1002,223,2,223,1006,224,554,1001,223,1,223,1107,226,677,224,1002,"
    "223,2,223,1006,224,569,1001,223,1,223,1108,677,677,224,102,2,223,223,1005,"
    "224,584,101,1,223,223,7,226,677,224,102,2,223,223,1006,224,599,1001,223,1,"
    "223,1108,226,677,224,102,2,223,223,1006,224,614,101,1,223,223,107,226,677,"
    "224,1002,223,2,223,1005,224,629,101,1,223,223,108,226,677,224,1002,223,2,"
    "223,1005,224,644,101,1,223,223,8,226,677,224,1002,223,2,223,1005,224,659,"
    "1001,223,1,223,107,226,226,224,1002,223,2,223,1006,224,674,101,1,223,223,"
    "4,223,99,226\n";

TEST(ComputerTest, Day2Part1Result)
{
    istringstream input_stream(day2_input_chars);
    auto input_memory = read_input(input_stream);
    input_memory[1] = 12;
    input_memory[2] = 2;
    Computer computer(input_memory);
    computer.run();
    ASSERT_EQ(computer.memory[0], 5290681);
}

TEST(ComputerTest, Day2Part2Result)
{
    istringstream input_stream(day2_input_chars);
    auto input_memory = read_input(input_stream);
    input_memory[1] = 57;
    input_memory[2] = 41;
    Computer computer(input_memory);
    computer.run();
    ASSERT_EQ(computer.memory[0], 19690720);
}

TEST(ComputerTest, Day5Part1Result)
{
    istringstream input_stream(day5_input_chars);
    istringstream user_input_stream("1\n");
    stringstream computer_output_stream;
    auto input_memory = read_input(input_stream);
    Computer computer(input_memory);
    computer.run(user_input_stream, computer_output_stream);

    vector<int32_t> output_ints;
    int output_int;
    while (computer_output_stream >> output_int) {
        output_ints.push_back(output_int);
    }
    // TODO: Verify all zeroes until end
    ASSERT_EQ(output_ints.back(), 6745903);
}

TEST(ComputerTest, Day5Part2Result)
{
    istringstream input_stream(day5_input_chars);
    istringstream user_input_stream("5\n");
    stringstream computer_output_stream;
    auto input_memory = read_input(input_stream);
    Computer computer(input_memory);
    computer.run(user_input_stream, computer_output_stream);

    vector<int32_t> output_ints;
    int output_int;
    while (computer_output_stream >> output_int) {
        output_ints.push_back(output_int);
    }
    ASSERT_EQ(output_ints.size(), 1UL);
    ASSERT_EQ(output_ints[0], 9168267);
}
