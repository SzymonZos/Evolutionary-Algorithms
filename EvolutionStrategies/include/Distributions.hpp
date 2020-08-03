#ifndef EVOLUTIONSTRATEGIES_DISTRIBUTIONS_HPP
#define EVOLUTIONSTRATEGIES_DISTRIBUTIONS_HPP

#include <random>

namespace ES {
struct Distributions {
    explicit Distributions(std::size_t noParents) : parent{0, noParents - 1} {}

    using params = std::normal_distribution<double>::param_type;
    std::normal_distribution<double> child;
    std::normal_distribution<double> standard{0.0, 1.0};
    std::uniform_real_distribution<double> coefficient{-10.0, 10.0};
    std::uniform_real_distribution<double> standardDeviation{0.0, 10.0};
    std::uniform_int_distribution<std::size_t> parent{};
};
} // namespace ES

#endif // EVOLUTIONSTRATEGIES_DISTRIBUTIONS_HPP
