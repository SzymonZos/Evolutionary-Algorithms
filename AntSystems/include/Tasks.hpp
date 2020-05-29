#ifndef ANTSYSTEMS_TASKS_HPP
#define ANTSYSTEMS_TASKS_HPP

#include "AntSystem.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
#include "matplotlibcpp.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <spdlog/sinks/rotating_file_sink.h>

namespace plt = matplotlibcpp;

template<std::size_t noAlleles>
std::tuple<IntArray<noAlleles>, double>
FirstTask(const DblMatrix<noAlleles, noAlleles>& distanceMatrix) {
    Timer timer;
    constexpr std::size_t noParents = 250;
    constexpr auto noOffspring = static_cast<std::size_t>(0.8 * noParents);
    const double mutationProbability = 0.1;
    const std::size_t tMax = 1000;
    AntSystem<noOffspring, noAlleles, noParents> algorithm{mutationProbability,
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
}

template<std::size_t noAlleles>
void PlotFirstTask(const DblArray<noAlleles>& x,
                   const DblArray<noAlleles>& y,
                   const IntArray<noAlleles>& result) {
    std::vector<double> xPlot, yPlot;
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

#endif // ANTSYSTEMS_TASKS_HPP
