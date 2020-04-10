#include "Operators.hpp"
#include "Types.hpp"
#include "spdlog/fmt/fmt.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <vector>

#define PRINT(x) std::cout << (#x) << ": " << (x) << std::endl;

static const double pi = std::acos(-1.0);

auto GetModelFromFile(std::size_t id) {
    std::array<std::vector<double>, 2> model;
    try {
        std::ifstream file{
            fmt::format("{}/models/model{}.txt", PROJECT_SOURCE_DIR, id)};
        file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        for (std::string line;
             file.peek() != EOF && std::getline(file, line);) {
            std::stringstream lineStream{line};
            double value = 0.0;
            for (auto& vector : model) {
                lineStream >> value;
                vector.push_back(value);
            }
        }
    } catch (const std::ifstream::failure& exception) {
        fmt::print("Exception: \"{}\" during opening/reading model{}.txt\n",
                   exception.what(),
                   id);
    }
    return model;
}

namespace Model {
enum InputOutput : std::size_t { input, output };
}

namespace Coeff {
enum Coefficient : std::size_t { a, b, c };
}

namespace StdDev {
enum StandardDeviation : std::size_t { a, b, c };
}

class Chromosome {
public:
    Chromosome& operator=(DblMatrix<2, 3>&& random) {
        for (std::size_t i = 0; i < 3; i++) {
            coefficients_[i] = random[i][0];
            standardDeviations_[i] = random[i][1];
        }
        return *this;
    }

    Chromosome& operator+=(DblMatrix<2, 3>& summand) {
        for (std::size_t i = 0; i < 3; i++) {
            coefficients_[i] += summand[i][0];
            standardDeviations_[i] *= summand[i][1];
        }
        return *this;
    }

    double operator()(double input) const {
        return coefficients_[Coeff::a] *
               (input * input -
                coefficients_[Coeff::b] *
                    std::cos(coefficients_[Coeff::c] * pi * input));
    }

    friend std::ostream& operator<<(std::ostream& stream,
                                    const Chromosome& data) {
        stream << "x: " << data.coefficients_;
        stream << "sigma: " << data.standardDeviations_;
        return stream;
    }

public: // needs encapsulation, ugly workaround
    DblArray<3> coefficients_{};
    DblArray<3> standardDeviations_{};
};

double MeanSquaredError(const std::array<std::vector<double>, 2>& model,
                        const Chromosome& chromosome) {
    double meanSquaredError = 0.0;
    for (std::size_t i = 0; i < model.front().size(); i++) {
        double error = chromosome(model[Model::input][i]) -
                       model[Model::output][i];
        meanSquaredError += (error * error);
    }
    //    std::cout << meanSquaredError << std::endl;
    return meanSquaredError / static_cast<double>(model.front().size());
}

void MutateChild(Chromosome& child) {
    using params = std::normal_distribution<double>::param_type;
    std::random_device randomDevice{};
    std::mt19937 rng{randomDevice()};
    std::normal_distribution childDistribution;
    std::normal_distribution standardDistribution{0.0, 1.0};
    const double n = 3.0;
    const double tau1 = 1.0 / std::sqrt(2.0 * n);
    const double tau2 = 1.0 / std::sqrt(2.0 * std::sqrt(n));

    DblMatrix<2, 3> random;
    for (std::size_t i = 0; i < 3; i++) {
        random[i][0] = childDistribution(
            rng,
            params{0.0, child.standardDeviations_[i]});
        random[i][1] = std::exp(tau1 * standardDistribution(rng)) +
                       std::exp(tau2 * standardDistribution(rng));
    }
    child += random;
}

int main() {
    const auto model = GetModelFromFile(9);
    const std::size_t noParents = model.front().size(); // mi
    const std::size_t noOffspring = 6 * noParents; // lambda 5 - 7

    std::vector<Chromosome> parents(noParents);
    std::vector<double> parentsEvaluation(noParents);

    // Initialize with uniform distribution
    std::random_device randomDevice{};
    std::mt19937 rng{randomDevice()};
    std::uniform_real_distribution<> coefficientsDistribution{-10.0, 10.0};
    std::uniform_real_distribution<> standardDeviationsDistribution{0.0, 10.0};
    std::uniform_int_distribution<> parentsDistribution{
        0,
        static_cast<int>(noParents - 1)};

    std::generate(parents.begin(), parents.end(), [&] {
        DblMatrix<2, 3> random;
        for (auto& params : random) {
            params[0] = coefficientsDistribution(rng);
            params[1] = standardDeviationsDistribution(rng);
        }
        return random;
    });

    std::generate(parentsEvaluation.begin(), parentsEvaluation.end(), [&] {
        static std::size_t i = 0;
        return MeanSquaredError(model, parents[i++]);
    });

    //    std::cout << parentsEvaluation << std::endl;

    std::vector<Chromosome> children(noOffspring);
    std::generate(children.begin(), children.end(), [&] {
        return parents[static_cast<std::size_t>(parentsDistribution(rng))];
    });

    std::for_each(children.begin(), children.end(), MutateChild);

    std::vector<double> childrenEvaluation(noParents);
    std::generate(childrenEvaluation.begin(), childrenEvaluation.end(), [&] {
        static std::size_t i = 0;
        return MeanSquaredError(model, children[i++]);
    });

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

    std::cout << sortedPopulation;

    return 0;
}