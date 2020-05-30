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

template<std::size_t noAnts>
auto FirstTask(const DblMatrix<noAnts, noAnts>& distanceMatrix) {
    Timer timer;
    const std::size_t tMax = 1000;
    AntSystem<noAnts> algorithm{tMax, distanceMatrix};
    return algorithm.GetResult();
}

template<std::size_t noAnts>
void SecondTask(const DblMatrix<noAnts, noAnts>& distanceMatrix) {
    Timer timer;
    auto logger = spdlog::rotating_logger_mt("results logger",
                                             "logs/results.csv",
                                             1048576 * 5,
                                             1,
                                             true);
    logger->info("minCostValue, noParents, n, mutationProbability");
    std::cout << distanceMatrix;
}

template<std::size_t noAnts>
void PlotFirstTask(const DblArray<noAnts>& x,
                   const DblArray<noAnts>& y,
                   const IntArray<noAnts>& result) {
    std::vector<double> xPlot, yPlot;
    xPlot.reserve(noAnts + 1);
    yPlot.reserve(noAnts + 1);
    for (std::size_t i = 0; i < noAnts; i++) {
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
