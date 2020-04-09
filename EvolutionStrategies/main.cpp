#include "Operators.hpp"
#include "Types.hpp"
#include "spdlog/fmt/fmt.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

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
enum class StandardDeviation : std::size_t { a, b, c };
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

    double operator()(double input) const {
        return coefficients_[Coeff::a] *
               (input * input -
                coefficients_[Coeff::b] *
                    std::cos(coefficients_[Coeff::c] * pi * input));
    }

    friend std::ostream& operator<<(std::ostream& stream,
                                    const Chromosome& data) {
        stream << "x: {";
        for (auto value : data.coefficients_) {
            stream << value << ", ";
        }
        stream << "\b\b}\nsigma: {";
        for (auto value : data.standardDeviations_) {
            stream << value << ", ";
        }
        stream << "\b\b}\nage: " << data.age_;
        return stream;
    }

private:
    DblArray<3> coefficients_{};
    DblArray<3> standardDeviations_{};
    std::size_t age_{};
};

double MeanSquaredError(const std::array<std::vector<double>, 2>& model,
                        const std::vector<Chromosome>& population) {
    double meanSquaredError = 0.0;
    for (std::size_t i = 0; i < population.size(); i++) {
        double error = population[i](model[Model::input][i]) -
                       model[Model::output][i];
        meanSquaredError += error * error;
    }
    return meanSquaredError / population.size();
}

int main() {
    const auto model = GetModelFromFile(9);
    const std::size_t noParents = model.front().size(); // mi
    const std::size_t noOffspring = 6 * noParents; // lambda 5 - 7

    std::vector<Chromosome> parents(noParents);

    // Initialize with uniform distribution
    std::random_device randomDevice{};
    std::mt19937 rng{randomDevice()};
    std::uniform_real_distribution<> xDistribution{-10.0, 10.0};
    std::uniform_real_distribution<> sigmaDistribution{0.0, 10.0};

    std::generate(parents.begin(), parents.end(), [&] {
        DblMatrix<2, 3> random;
        for (auto& params : random) {
            params[0] = xDistribution(rng);
            params[1] = sigmaDistribution(rng);
        }
        return random;
    });

    std::cout << parents << std::endl;

    //    std::cout << parents << std::endl;
    std::cout << MeanSquaredError(model, parents);
    return 0;
}