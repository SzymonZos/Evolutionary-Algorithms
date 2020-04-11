#ifndef EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
#define EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP

#include "Chromosome.hpp"
#include "Distributions.hpp"

namespace ES {
class EvolutionStrategies {
private:
    std::random_device randomDevice_{};
    std::mt19937 rng_{randomDevice_()};
    Distributions distributions_{101};
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
