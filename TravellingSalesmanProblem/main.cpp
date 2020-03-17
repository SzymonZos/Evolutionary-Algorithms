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
constexpr double pm = 0.2; // need a better name for this variable
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
    T offspring = {};
    std::size_t index = 0;
    typename T::const_iterator iter;
    using type = typename T::value_type;
    for (std::size_t i = 0; i < parent1.size(); i++) {
        offspring[index] = parent1[index];
        iter = std::find(parent1.cbegin(), parent1.cend(), parent2[index]);
        if (iter == parent1.cbegin()) {
            CopyIfOutput(parent2.cbegin(), parent2.cend(), offspring.begin(),
                         [=](type value){ return !value; });
            break;
        }
        index = std::distance(parent1.cbegin(), iter);
    }
    return offspring;
}

int main() {
    constexpr IntArray<noAlleles> x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    constexpr IntArray<noAlleles> y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    constexpr auto distanceMatrix = CalculateDistanceMatrix(x, y);

    auto population = CreateInitialPopulation();
    auto parentCostValues = CalculateCostValues(population, distanceMatrix);
    auto chosenParents = SelectParents(population, parentCostValues);


////    std::array<int32_t, 5> parent1 = {2, 4, 5, 1, 3};
////    std::array<int32_t, 5> parent2 = {1, 5, 4, 2, 3};
////
////    auto offspring1 = CrossoverParents(parent1, parent2);
////    auto offspring2 = CrossoverParents(parent2, parent1);
////

    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());

    IntMatrix<noAlleles, noOffspring> offspring = {};
    std::uniform_int_distribution<> intDistribution(0, 199);
    std::size_t indexOfParent_1 = 0;
    std::size_t indexOfParent_2 = 0;
    for (std::size_t i = 0; i < noOffspring; i += 2) {
        indexOfParent_1 = intDistribution(rng);
        indexOfParent_2 = intDistribution(rng);
        offspring[i] = CrossoverParents(chosenParents[indexOfParent_1], chosenParents[indexOfParent_2]);
        offspring[i + 1] = CrossoverParents(chosenParents[indexOfParent_2], chosenParents[indexOfParent_1]);
    }
    for (auto& child : offspring){
        std::cout << "OFFSPRING:" << child << std::endl;
    }
//
//    double mutationProbability = 0.2;
//    std::uniform_real_distribution<> probabilityDistribution(0.0, 1.0);
//    std::uniform_int_distribution<> mutationDistribution(0, 9);
//    std::size_t mutatingGen_1 = 0;
//    std::size_t mutatingGen_2 = 0;
//    std::size_t tmp = 0;
//    std::size_t mutationCount = 0;
//    double mutationChance = 0.0;
//    for (auto& child : offspring){
//        mutationChance = probabilityDistribution(rng);
//        if (mutationChance < mutationProbability) {
//            std::cout << "BEFORE MUTATION: " << child << std::endl;
//            mutatingGen_1 = mutationDistribution(rng);
//            mutatingGen_2 = mutationDistribution(rng);
//            tmp = child[mutatingGen_1];
//            child[mutatingGen_1] = child[mutatingGen_2];
//            child[mutatingGen_2] = tmp;
//            std::cout << "AFTER MUTATION:  " << child << std::endl;
//            mutationCount++;
//        }
//    }
//    std::cout << "Number of mutated: " << mutationCount << std::endl;
//
//    std::array<double, noOffspring> offspringCostValues = CalculateCostValue<std::array<arrayInt, noOffspring>, std::array<double, noOffspring>>(offspring, distanceMatrix);
//    for (const auto& cost : offspringCostValues) {
//        std::cout << "Cost value is: " << cost << std::endl;
//    }
//
//    std::array<arrayInt, noParents> newPopulation = population;
//    int noChangedCasesDebug=0;
//    for (std::size_t i=0; i<noOffspring; i++){
//        auto maxCostPosition = std::distance(parentCostValues.begin(),
//                                             std::max_element(parentCostValues.begin(), parentCostValues.end()));
//        if(offspringCostValues[i]<parentCostValues[maxCostPosition]){
//            newPopulation[maxCostPosition]=offspring[i];
//            parentCostValues[maxCostPosition]=offspringCostValues[i];
//            noChangedCasesDebug++;
//        }
//    }
//    std::cout << noChangedCasesDebug << std::endl;
//    population=newPopulation;
    /*teraz tylko zapetlic i algorytm gotowy kappa*/
    return 0;
}
