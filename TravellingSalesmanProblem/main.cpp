#include <iostream>
#include "Utils.hpp"
#include "GeneticAlgorithm.hpp"
#include "Timer.hpp"

// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]
// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

int main() {
    Timer timer;
    constexpr std::size_t noAlleles = 10;
    constexpr std::size_t noParents = 250;
    constexpr double n = 0.8;
    constexpr std::size_t noOffspring = n * noParents;
    constexpr IntArray<noAlleles> x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    constexpr IntArray<noAlleles> y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    constexpr auto distanceMatrix = CalculateDistanceMatrix(x, y);

    const double mutationProbability = 0.2;
    const std::size_t tMax = 1000;

    GeneticAlgorithm<noOffspring, noAlleles, noParents> algorithm{
        mutationProbability, tMax, distanceMatrix};

    return 0;
}
