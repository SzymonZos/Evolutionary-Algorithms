#include <iostream>
#include "Utils.hpp"
#include "GeneticAlgorithm.hpp"
#include "Timer.hpp"

#define GENETIC_ALGORITHM(noParents, n, mutationProbability) \
    { \
        constexpr std::size_t noOffspring = (n) * (noParents); \
        GeneticAlgorithm<noOffspring, noAlleles, noParents> algorithm{ \
        (mutationProbability), tMax, distanceMatrix}; \
    }

// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]
// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

template<std::size_t noAlleles>
void FirstTask(const DblMatrix<noAlleles, noAlleles>& distanceMatrix ) {
    Timer timer;
    constexpr std::size_t noParents = 250;
    constexpr std::size_t noOffspring = 0.8 * noParents;
    const double mutationProbability = 0.1;
    const std::size_t tMax = 1000;
    GeneticAlgorithm<noOffspring, noAlleles, noParents> algorithm{
            mutationProbability, tMax, distanceMatrix};
}

template<std::size_t noAlleles>
void SecondTask(const DblMatrix<noAlleles, noAlleles>& distanceMatrix) {
    Timer timer;
    constexpr std::size_t size = 3;
    constexpr IntArray<size> noParents = {100, 300, 500};
    constexpr DblArray<size> n = {0.5, 0.7, 0.9};
    const DblArray<size> mutationProbabilityVec = {0.1, 0.3, 0.5};
    const std::size_t tMax = 1000;

    for (std::size_t i = 0; i < 10; i ++) {
        for (auto mutationProbability : mutationProbabilityVec) {
            GENETIC_ALGORITHM(100, 0.5, mutationProbability)
            GENETIC_ALGORITHM(100, 0.7, mutationProbability)
            GENETIC_ALGORITHM(100, 0.9, mutationProbability)
            GENETIC_ALGORITHM(300, 0.5, mutationProbability)
            GENETIC_ALGORITHM(300, 0.7, mutationProbability)
            GENETIC_ALGORITHM(300, 0.9, mutationProbability)
            GENETIC_ALGORITHM(500, 0.5, mutationProbability)
            GENETIC_ALGORITHM(500, 0.7, mutationProbability)
            GENETIC_ALGORITHM(500, 0.9, mutationProbability)
        }
    }
}

int main() {
    constexpr std::size_t noAlleles = 10;
    constexpr IntArray<noAlleles> x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    constexpr IntArray<noAlleles> y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    constexpr auto distanceMatrix = CalculateDistanceMatrix(x, y);
    FirstTask(distanceMatrix);
    SecondTask(distanceMatrix);

    return 0;
}
