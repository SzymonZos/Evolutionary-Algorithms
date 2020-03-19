#include <iostream>
#include <random>
#include "Math.hpp"
#include "Algorithm.hpp"
#include "Types.hpp"
#include "Operators.hpp"

// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]
// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

constexpr const std::size_t noAlleles = 10;
constexpr std::size_t noParents = 250;
constexpr double n = 0.8;
constexpr std::size_t noOffspring = n * noParents;
constexpr double mutationProbability = 0.2;
constexpr std::size_t tMax = 1000;

template<typename T>
constexpr DblMatrix<noAlleles, noAlleles> CalculateDistanceMatrix(const T& x,
                                                                  const T& y) {
    DblMatrix<noAlleles, noAlleles> distanceMatrix = {};
    for (std::size_t i = 0; i < noAlleles; i++) {
        for (std::size_t j = 0; j < noAlleles; j++) {
            distanceMatrix[i][j] = Distance(x[i], x[j], y[i], y[j]);
        }
    }
    return distanceMatrix;
}

IntMatrix<noAlleles, noParents> CreateInitialPopulation() {
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());

    IntMatrix<noAlleles, noParents> population = {};
    IntArray<noAlleles> cities;
    std::iota(cities.begin(), cities.end(), 0);
    for (auto& parent : population) {
        parent = cities;
        std::shuffle(parent.begin(), parent.end(), randomNumberGenerator);
    }
    return population;
}

template<typename Pop, typename Dist>
auto CalculateCostValues(const Pop& population, const Dist& distanceMatrix) {
    DblArray<population.size()> costValues = {};
    std::size_t idx = 0;
    for (auto& chromosome : population) {
        costValues[idx] += distanceMatrix[chromosome.front()][chromosome.back()];
        for (std::size_t i = 0; i < chromosome.size() - 1; i++) {
            costValues[idx] += distanceMatrix[chromosome[i]][chromosome[i + 1]];
        }
        idx++;
    }
    return costValues;
}

template<typename Pop, typename Cost>
auto SelectParents(const Pop& parents, const Cost& costValues) {
    IntMatrix<noAlleles, noOffspring> chosenParents = {};
    auto maxCost = std::max_element(costValues.begin(), costValues.end());
    double ts = std::accumulate(costValues.begin(), costValues.end(), 0.0,
                                [&](double prev, double curr) {
                                return prev + *maxCost - curr; });
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    std::uniform_real_distribution<> realDistribution(0.0, ts);
    for (auto& chosenParent : chosenParents) {
        double randomNumber = realDistribution(randomNumberGenerator);
        double ti = 0;
        for (std::size_t i = 0; i < noParents; i++) {
            ti += *maxCost - costValues[i];
            if (ti >= randomNumber) {
                chosenParent = parents[i];
                break;
            }
        }
    }
    return chosenParents;
}

template<typename T>
T CrossoverParents(const T& parent1, const T& parent2) {
    using type = typename T::value_type;
    type dummy = -1;
    T offspring = {};
    std::fill(offspring.begin(), offspring.end(), dummy);
    std::size_t index = 0;
    typename T::const_iterator iter;
    for (std::size_t i = 0; i < parent1.size(); i++) {
        offspring[index] = parent1[index];
        iter = std::find(parent1.cbegin(), parent1.cend(), parent2[index]);
        if (iter == parent1.cbegin()) {
            CopyIfOutput(parent2.cbegin(), parent2.cend(), offspring.begin(),
                         [=](type value){ return value == dummy; });
            break;
        }
        index = std::distance(parent1.cbegin(), iter);
    }
    return offspring;
}

template<typename T>
T GenerateOffspring(const T& parents) {
    T offspring = {};
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());
    std::uniform_int_distribution<> intDistribution(0, noOffspring - 1);
    IntArray<2> index = {};
    for (std::size_t i = 0; i < noOffspring; i += 2) {
        std::generate(index.begin(), index.end(),
                      [&] { return intDistribution(rng); });
        offspring[i] = CrossoverParents(parents[index[0]], parents[index[1]]);
        offspring[i + 1] = CrossoverParents(parents[index[1]], parents[index[0]]);
    }
    return offspring;
}

template<typename T>
void MutateOffspring(T&& offspring) {
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());
    std::uniform_real_distribution<> probabilityDistribution(0.0, 1.0);
    std::uniform_int_distribution<> mutationDistribution(0, noAlleles - 1);
    IntArray<2> index = {};
    for (auto& child : offspring){
        if (probabilityDistribution(rng) < mutationProbability) {
            std::generate(index.begin(), index.end(),
                          [&] { return mutationDistribution(rng); });
            std::swap(child[index[0]], child[index[1]]);
        }
    }
}

int main() {
    constexpr IntArray<noAlleles> x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    constexpr IntArray<noAlleles> y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    constexpr auto distanceMatrix = CalculateDistanceMatrix(x, y);
    auto population = CreateInitialPopulation();
    double minCostValue = 0;

    for (std::size_t i = 0; i < tMax; i++) {
        auto parentCostValues = CalculateCostValues(population, distanceMatrix);
        auto chosenParents = SelectParents(population, parentCostValues);
        auto offspring = GenerateOffspring(chosenParents);
        MutateOffspring(offspring);
        auto offspringCostValues = CalculateCostValues(offspring, distanceMatrix);

        std::multimap<double, IntArray<noAlleles>> sortedPopulation;
        for (std::size_t i = 0; i < noParents; i++) {
            sortedPopulation.insert({parentCostValues[i], population[i]});
            if (i < noOffspring) {
                sortedPopulation.insert({offspringCostValues[i], offspring[i]});
            }
        }
        auto it = sortedPopulation.begin();
        std::generate(population.begin(), population.end(),
                      [&] { return (it++)->second; });
        minCostValue = sortedPopulation.begin()->first;
    }
    std::cout << "Best route is: " << population[0] <<
    " with cost value: " << minCostValue << std::endl;
    return 0;
}
