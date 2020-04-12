#ifndef EVOLUTIONARYALGORITHMS_POPULATION_HPP
#define EVOLUTIONARYALGORITHMS_POPULATION_HPP

#include "Chromosome.hpp"
#include <map>
#include <vector>

namespace ES {

template<std::size_t noCoefficients>
struct Population {
    Population(std::size_t size, std::size_t offspringFactor) :
        noParents{size},
        noOffspring{offspringFactor * noParents},
        parents{noParents},
        offspring{noOffspring},
        evaluationOfParents{noParents},
        evaluationOfOffspring{noOffspring} {}

    std::size_t noParents;
    std::size_t noOffspring;
    std::vector<Chromosome<noCoefficients>> parents;
    std::vector<Chromosome<noCoefficients>> offspring;
    std::vector<double> evaluationOfParents;
    std::vector<double> evaluationOfOffspring;
    std::multimap<double, Chromosome<noCoefficients>> sorted{};
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_POPULATION_HPP
