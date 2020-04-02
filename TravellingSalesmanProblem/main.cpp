#include "GeneticAlgorithm.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
#include "matplotlibcpp.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <spdlog/sinks/rotating_file_sink.h>

namespace plt = matplotlibcpp;

#define GENETIC_ALGORITHM(noParents, n, mutationProbability) \
    { \
        constexpr std::size_t noOffspring = (n) * (noParents); \
        GeneticAlgorithm<noOffspring, noAlleles, noParents> algorithm{ \
            (mutationProbability), \
            tMax, \
            distanceMatrix}; \
        auto [results, minCostValue] = algorithm.GetResult(); \
        logger->info("{:.4f}, {}, {}, {}", \
                     minCostValue, \
                     noParents, \
                     n, \
                     mutationProbability); \
    }

// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]
// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

template<std::size_t noAlleles>
std::tuple<IntArray<noAlleles>, double>
FirstTask(const DblMatrix<noAlleles, noAlleles>& distanceMatrix) {
    Timer timer;
    constexpr std::size_t noParents = 250;
    constexpr auto noOffspring = static_cast<std::size_t>(0.8 * noParents);
    const double mutationProbability = 0.1;
    const std::size_t tMax = 1000;
    GeneticAlgorithm<noOffspring, noAlleles, noParents> algorithm{
        mutationProbability,
        tMax,
        distanceMatrix};
    return algorithm.GetResult();
}

template<std::size_t noAlleles>
void SecondTask(const DblMatrix<noAlleles, noAlleles>& distanceMatrix) {
    Timer timer;
    constexpr std::size_t size = 3;
    const DblArray<size> mutationProbabilityVec = {0.1, 0.3, 0.5};
    const std::size_t tMax = 1000;
    auto logger = spdlog::rotating_logger_mt("results logger",
                                             "logs/results.csv",
                                             1048576 * 5,
                                             1,
                                             true);
    logger->info("minCostValue, noParents, n, mutationProbability");
    for (std::size_t i = 0; i < 10; i++) {
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

template<std::size_t noAlleles>
void PlotFirstTask(const IntArray<noAlleles>& x,
                   const IntArray<noAlleles>& y,
                   const IntArray<noAlleles>& result) {
    std::vector<std::size_t> xPlot, yPlot;
    xPlot.reserve(noAlleles + 1);
    yPlot.reserve(noAlleles + 1);
    for (std::size_t i = 0; i < noAlleles; i++) {
        xPlot.push_back(x[result[i]]);
        yPlot.push_back(y[result[i]]);
        plt::annotate(std::to_string(result[i]), xPlot[i] + 0.2, yPlot[i]);
    }
    xPlot.push_back(x[result[0]]);
    yPlot.push_back(y[result[0]]);
    plt::scatter(xPlot, yPlot, 100.0, {{"c", "red"}, {"marker", "*"}});
    plt::plot(xPlot, yPlot);
    plt::show();
}

int main() {
    constexpr std::size_t noAlleles = 10;
    constexpr IntArray<noAlleles> x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    constexpr IntArray<noAlleles> y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    constexpr auto distanceMatrix = CalculateDistanceMatrix(x, y);
    auto [result, minCostValue] = FirstTask(distanceMatrix);
    PlotFirstTask(x, y, result);
    SecondTask(distanceMatrix);

    return 0;
}
