#include "Operators.hpp"
#include "Types.hpp"
#include "spdlog/fmt/fmt.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

static const double pi = std::acos(-1.0);

struct v;

template<std::size_t size>
using P = std::array<v, size>;

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

struct v {
    DblArray<3> x{};
    DblArray<3> sigma{};
    std::size_t age{};

    v& operator=(DblMatrix<2, 3>&& random) {
        for (std::size_t i = 0; i < 3; i++) {
            x[i] = random[i][0];
            sigma[i] = random[i][1];
        }
        return *this;
    }

    double operator()(double input) const {
        return x[0] * (input * input - x[1] * std::cos(x[2] * pi * input));
    }

    friend std::ostream& operator<<(std::ostream& stream, const v& data) {
        stream << "x: {";
        for (auto value : data.x) {
            stream << value << ", ";
        }
        stream << "\b\b}\nsigma: {";
        for (auto value : data.sigma) {
            stream << value << ", ";
        }
        stream << "\b\b}\nage: " << data.age;
        return stream;
    }
};

template<std::size_t size>
double MeanSquaredError(const std::vector<DblArray<2>>& model,
                        const P<size>& population) {
    double ts = std::accumulate(model.begin(), model.end(), 0.0);
    std::cout << population << std::endl;
    return ts;
}

int main() {
    auto model = GetModelFromFile(9);
    std::cout << model << std::endl;
    constexpr std::size_t mi = 100; // noParents
    constexpr std::size_t lambda = 6 * mi; // noOffspring 5 - 7
    std::cout << lambda << std::endl;
    P<mi> parents;

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

    //    std::cout << parents << std::endl;
//    MeanSquaredError(model, parents);
    return 0;
}