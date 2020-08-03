#ifndef EVOLUTIONSTRATEGIES_POPULATION_HPP
#define EVOLUTIONSTRATEGIES_POPULATION_HPP

#include "Chromosome.hpp"
#include <map>
#include <vector>

namespace ES {
enum class StrategyType : std::size_t { parentsAndOffspring, offspring };

struct PopulationSize {
    std::size_t noParents = 100;
    double offspringFactor = 6;
    StrategyType strategyType = StrategyType::parentsAndOffspring;
} defaultPop;

template<std::size_t noCoefficients>
struct Population {
    explicit Population(PopulationSize size) :
        noParents{size.noParents},
        noOffspring{static_cast<std::size_t>(size.offspringFactor *
                                             static_cast<double>(noParents))},
        parents(noParents),
        offspring(noOffspring),
        parentsEvaluation(noParents),
        offspringEvaluation(noOffspring),
        strategyType{size.strategyType} {}

    std::size_t noParents;
    std::size_t noOffspring;
    std::vector<Chromosome<noCoefficients>> parents;
    std::vector<Chromosome<noCoefficients>> offspring;
    std::vector<double> parentsEvaluation;
    std::vector<double> offspringEvaluation;
    std::multimap<double, Chromosome<noCoefficients>> sorted{};
    StrategyType strategyType;
};
} // namespace ES

#endif // EVOLUTIONSTRATEGIES_POPULATION_HPP
