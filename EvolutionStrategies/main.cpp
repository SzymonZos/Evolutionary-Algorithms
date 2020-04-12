#include "EvolutionStrategies.hpp"
#include "Model.hpp"
#include "Timer.hpp"
#include "matplotlibcpp.h"
#include <iostream>

namespace plt = matplotlibcpp;

template<std::size_t noCoefficients>
auto FirstTask(const std::array<std::vector<double>, 2>& model) {
    Timer timer;
    ES::EvolutionStrategies<noCoefficients> strategies{
        model,
        6,
        ES::StrategyType::offspring};
    return strategies.GetResult();
}

template<std::size_t noCoefficients>
void PlotResults(const std::array<std::vector<double>, 2>& model,
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
    ES::Model model{9};
    constexpr std::size_t noCoefficients = 3;
    auto results = FirstTask<noCoefficients>(model.GetModel());
    std::cout << results << std::endl;
    PlotResults<noCoefficients>(model.GetModel(), results);
    return 0;
}
