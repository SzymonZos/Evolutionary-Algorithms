#include "EvolutionStrategies.hpp"
#include "Model.hpp"
#include "Timer.hpp"
#include "matplotlibcpp.h"
#include <iostream>

namespace plt = matplotlibcpp;

template<std::size_t noCoefficients>
auto FirstTask(const ES::Model::Type& model) {
    Timer timer;
    ES::EvolutionStrategies<noCoefficients> strategies{
        model,
        {200, 7, ES::StrategyType::offspring},
        {0.3, 5, 100}};

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
