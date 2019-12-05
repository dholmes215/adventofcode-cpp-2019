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

} //namespace intcode;