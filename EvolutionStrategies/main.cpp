#include "EvolutionStrategies.hpp"
#include "Model.hpp"
#include "Timer.hpp"
#include "matplotlibcpp.h"
#include <iostream>

namespace plt = matplotlibcpp;

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

int main() {
    Timer timer;
    ES::Model model{9};
    constexpr std::size_t noCoefficients = 3;
    ES::EvolutionStrategies<noCoefficients> strategies{
        model.GetModel(),
        6,
        ES::StrategyType::parentsAndOffspring};
    std::cout << strategies.GetResult();
    return 0;
}
