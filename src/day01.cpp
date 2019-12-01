// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using Mass = int32_t;

vector<Mass> ReadInput(istream & stream)
{
    vector<Mass> out;
    int32_t input;
    while (stream >> input) {
        out.push_back(input);
    }
    return out;
}

Mass FuelRequirementNaive(const Mass mass) { return max(mass / 3 - 2, 0); }

Mass FuelRequirementActual(const Mass mass)
{
    Mass naive = FuelRequirementNaive(mass);
    if (naive == 0) {
        return 0;
    }
    return naive + FuelRequirementActual(naive);
}

template <typename FuelCalcFunc>
vector<Mass> GetFuelRequirements(const vector<Mass> & masses,
                                 FuelCalcFunc fuelRequirementFunc)
{
    vector<Mass> out;
    out.reserve(masses.size());
    for (const Mass mass : masses) {
        out.push_back(fuelRequirementFunc(mass));
    }
    return out;
}

template <typename Container> auto AddAll(const Container & container)
{
    return accumulate(begin(container), end(container), 0);
}

int main(int /*argc*/, char ** /*argv*/)
{
    const vector<Mass> masses = ReadInput(cin);

    const vector<Mass> fuelReqsNaive =
        GetFuelRequirements(masses, FuelRequirementNaive);
    const Mass totalFuelReqNaive = AddAll(fuelReqsNaive);
    cout << "Total fuel requirement (naive): " << totalFuelReqNaive << '\n';

    const vector<Mass> fuelReqsActual =
        GetFuelRequirements(masses, FuelRequirementActual);
    const Mass totalFuelReqActual = AddAll(fuelReqsActual);
    cout << "Total fuel requirement (actual): " << totalFuelReqActual << '\n';

    return 0;
}
