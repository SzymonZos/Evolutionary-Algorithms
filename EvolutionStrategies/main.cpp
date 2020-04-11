#include "EvolutionStrategies.hpp"
#include "Model.hpp"
#include <iostream>
#include <map>
#include <random>
#include <vector>

template<std::size_t noCoefficients>
double MeanSquaredError(const std::array<std::vector<double>, 2>& model,
                        const ES::Chromosome<noCoefficients>& chromosome) {
    double meanSquaredError = 0.0;
    for (std::size_t i = 0; i < model.front().size(); i++) {
        double error = chromosome(model[ES::in][i]) - model[ES::out][i];
        meanSquaredError += (error * error);
    }
    return meanSquaredError / static_cast<double>(model.front().size());
}

template<std::size_t noCoefficients>
void MutateChild(ES::Chromosome<noCoefficients>& child) {
    using params = std::normal_distribution<double>::param_type;
    std::random_device randomDevice{};
    std::mt19937 rng{randomDevice()};
    std::normal_distribution childDistribution;
    std::normal_distribution standardDistribution{0.0, 1.0};
    const double n = 3.0;
    const double tau1 = 1.0 / std::sqrt(2.0 * n);
    const double tau2 = 1.0 / std::sqrt(2.0 * std::sqrt(n));

    ES::Chromosome<noCoefficients> random;
    std::size_t i = 0;
    std::generate(random[ES::coeffs].begin(), random[ES::coeffs].end(), [&] {
        return childDistribution(rng, params{0.0, child[ES::stddevs][i++]});
    });
    std::generate(random[ES::stddevs].begin(), random[ES::stddevs].end(), [&] {
        return std::exp(tau1 * standardDistribution(rng)) +
               std::exp(tau2 * standardDistribution(rng));
    });
    child += random;
}

int main() {
    ES::Model heh{9};
    const auto model = heh.GetModel();
    const std::size_t noParents = model.front().size(); // mi
    const std::size_t noOffspring = 6 * noParents; // lambda 5 - 7
    constexpr std::size_t noCoefficients = 3;
    using Chromosome = ES::Chromosome<noCoefficients>;

    std::vector<Chromosome> parents(noParents);
    std::vector<double> parentsEvaluation(noParents);

    // Initialize with uniform distribution
    std::random_device randomDevice{};
    std::mt19937 rng{randomDevice()};
    std::uniform_real_distribution coefficientsDistribution{-10.0, 10.0};
    std::uniform_real_distribution standardDeviationsDistribution{0.0, 10.0};
    std::uniform_int_distribution parentsDistribution{
        0,
        static_cast<int>(noParents - 1)};

    std::generate(parents.begin(), parents.end(), [&] {
        ES::Chromosome<noCoefficients> random;
        std::generate(random[ES::coeffs].begin(),
                      random[ES::coeffs].end(),
                      [&] { return coefficientsDistribution(rng); });
        std::generate(random[ES::stddevs].begin(),
                      random[ES::stddevs].end(),
                      [&] { return standardDeviationsDistribution(rng); });
        return random;
    });
    std::size_t idx = 0;
    std::generate(parentsEvaluation.begin(), parentsEvaluation.end(), [&] {
        return MeanSquaredError(model, parents[idx++]);
    });

    std::cout << parentsEvaluation << std::endl;

    std::vector<Chromosome> children(noOffspring);
    std::generate(children.begin(), children.end(), [&] {
        return parents[static_cast<std::size_t>(parentsDistribution(rng))];
    });

    std::for_each(children.begin(),
                  children.end(),
                  MutateChild<noCoefficients>);

    std::vector<double> childrenEvaluation(noOffspring);
    std::generate(childrenEvaluation.begin(), childrenEvaluation.end(), [&] {
        static std::size_t i = 0;
        return MeanSquaredError(model, children[i++]);
    });

    std::cout << childrenEvaluation << std::endl;

    std::multimap<double, Chromosome> sortedPopulation{};
    double minCostValue = 0.0;

    for (std::size_t i = 0; i < noOffspring; i++) {
        sortedPopulation.insert({childrenEvaluation[i], children[i]});
        if (i < noParents) {
            sortedPopulation.insert({parentsEvaluation[i], parents[i]});
        }
    }
    auto it = sortedPopulation.begin();
    std::generate(parents.begin(), parents.end(), [&] {
        return (it++)->second;
    });
    minCostValue = sortedPopulation.begin()->first;

    //    std::cout << sortedPopulation << minCostValue;

    return 0;
}
