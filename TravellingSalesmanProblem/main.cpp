#include "GeneticAlgorithm.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
#include "matplotlibcpp.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <iostream>

namespace plt = matplotlibcpp;

#define GENETIC_ALGORITHM(noParents, n, mutationProbability) \
    { \
        constexpr std::size_t noOffspring = (n) * (noParents); \
        GeneticAlgorithm<noOffspring, noAlleles, noParents> algorithm{ \
            (mutationProbability), \
            tMax, \
            distanceMatrix}; \
    }

// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]
// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

template<std::size_t noAlleles>
void FirstTask(const DblMatrix<noAlleles, noAlleles>& distanceMatrix) {
    Timer timer;
    constexpr std::size_t noParents = 250;
    constexpr auto noOffspring = static_cast<std::size_t>(0.8 * noParents);
    const double mutationProbability = 0.1;
    const std::size_t tMax = 1000;
    GeneticAlgorithm<noOffspring, noAlleles, noParents> algorithm{
        mutationProbability,
        tMax,
        distanceMatrix};
}

template<std::size_t noAlleles>
void SecondTask(const DblMatrix<noAlleles, noAlleles>& distanceMatrix) {
    Timer timer;
    constexpr std::size_t size = 3;
    constexpr IntArray<size> noParents = {100, 300, 500};
    constexpr DblArray<size> n = {0.5, 0.7, 0.9};
    const DblArray<size> mutationProbabilityVec = {0.1, 0.3, 0.5};
    const std::size_t tMax = 1000;

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

int main() {
    constexpr std::size_t noAlleles = 10;
    constexpr IntArray<noAlleles> x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    constexpr IntArray<noAlleles> y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    constexpr auto distanceMatrix = CalculateDistanceMatrix(x, y);
    FirstTask(distanceMatrix);
    //    SecondTask(distanceMatrix);
    plt::plot({1, 3, 2, 4});
    plt::show();
    auto console = spdlog::stdout_color_mt("console");
    spdlog::get("console")->info("Hello spdlog!");
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical(
        "Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}",
        42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", {});
    SPDLOG_DEBUG("Some debug message");

    return 0;
}
