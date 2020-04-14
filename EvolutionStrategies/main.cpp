#include "EvolutionStrategies.hpp"
#include "Model.hpp"
#include "Timer.hpp"
#include "matplotlibcpp.h"
#include <iostream>
#include <spdlog/sinks/rotating_file_sink.h>

namespace plt = matplotlibcpp;

template<std::size_t noCoefficients>
auto FirstTask(const ES::Model::Type& model) {
    auto logger = spdlog::rotating_logger_mt("results",
                                             "logs/results.csv",
                                             1048576 * 5,
                                             1,
                                             true);
    Timer timer;
    ES::EvolutionStrategies<noCoefficients> strategies{
        model,
        {200, 7, ES::StrategyType::offspring},
        {0.3, 5, 100},
        logger};
    return strategies.GetResults();
}

template<std::size_t noCoefficients>
void PlotResults(const ES::Model::Type& model,
                 const ES::Chromosome<noCoefficients>& result) {
    std::vector<double> resultOutput(model.front().size());
    std::size_t i = 0;
    std::generate(resultOutput.begin(), resultOutput.end(), [&] {
        return result(model[ES::in][i++]);
    });
    plt::plot(model[ES::in], resultOutput, {{"c", "blue"}});
    plt::plot(model[ES::in], model[ES::out], {{"c", "red"}});
    plt::show();
}

int main() {
    ES::Model rawModel{9};
    const auto model = rawModel.GetModel();
    constexpr std::size_t noCoefficients = 3;
    auto [results, mse, iterations] = FirstTask<noCoefficients>(model);
    std::cout << results << "mse: " << mse << "\n"
              << "number of iterations: " << iterations;
    PlotResults<noCoefficients>(model, results);
    return 0;
}
