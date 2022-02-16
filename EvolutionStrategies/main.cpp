#include "EvolutionStrategies.hpp"
#include "Model.hpp"
#include "Timer.hpp"
#include "matplotlibcpp.h"
#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/sinks/rotating_file_sink.h>

namespace plt = matplotlibcpp;

template<std::size_t noCoefficients>
auto FirstTask(const ES::Model::Type& model) {
    Timer timer;
    ES::EvolutionStrategies<noCoefficients> strategies{
        model,
        {100, 6, ES::StrategyType::offspring},
        {0.3, 10, 1000}};
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

template<std::size_t noCoefficients>
void SecondTask(const ES::Model::Type& model) {
    auto logger = spdlog::rotating_logger_mt("results",
                                             "logs/results.csv",
                                             1048576 * 5,
                                             1,
                                             true);
    std::vector<std::size_t> parents{2, 5, 10, 50, 100, 500};
    std::vector<ES::StrategyType> strategies{
        ES::StrategyType::parentsAndOffspring,
        ES::StrategyType::offspring};
    Timer timer;
    std::for_each(parents.begin(), parents.end(), [&](auto parent) {
        std::for_each(strategies.begin(), strategies.end(), [&](auto strat) {
            return ES::EvolutionStrategies<noCoefficients>{model,
                                                           {parent, 6, strat},
                                                           {0.3, 10, 1000},
                                                           logger};
        });
    });
}

template<std::size_t noCoefficients>
void MultipleTests(const ES::Model::Type& model, const std::size_t noTests) {
    std::vector<ES::StrategyType> strategies{
        ES::StrategyType::parentsAndOffspring,
        ES::StrategyType::offspring};
    Timer timer;
    auto logger = spdlog::rotating_logger_mt("comparison",
                                             "logs/comparison.csv",
                                             1048576 * 5,
                                             1,
                                             true);
    for (std::size_t test = 0; test < noTests; test++) {
        std::for_each(strategies.begin(), strategies.end(), [&](auto strat) {
            return ES::EvolutionStrategies<noCoefficients>{model,
                                                           {500, 6, strat},
                                                           {0.3, 10, 1000},
                                                           logger};
        });
    }
}

auto parse_args(int argc, char** argv) {
    try {
        cxxopts::Options options(argv[0], "Evolution strategies");
        options.positional_help("[optional args]").show_positional_help();

        // clang-format off
        options
            .allow_unrecognised_options()
            .add_options()
                ("p,plot", "Enable plotting");
        // clang-format on
        return options.parse(argc, argv);
    } catch (const cxxopts::OptionException& e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        std::exit(1);
    }
}

int main(int argc, char** argv) {
    auto options = parse_args(argc, argv);
    ES::Model rawModel{9};
    const auto model = rawModel.GetModel();
    constexpr std::size_t noCoefficients = 3;
    auto [results, mse, iterations] = FirstTask<noCoefficients>(model);
    std::cout << results << '\n'
              << "mse: " << mse << '\n'
              << "number of iterations: " << iterations << '\n';
    if (options.count("plot")) {
        PlotResults<noCoefficients>(model, results);
    }
    SecondTask<noCoefficients>(model);
    MultipleTests<noCoefficients>(model, 10);
    return 0;
}
