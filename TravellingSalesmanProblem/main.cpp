#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include "Operators.hpp"
// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]
// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

const std::size_t noCities = 10;

typedef std::array<int32_t, noCities> arrayInt;
typedef std::array<double, noCities> arrayDbl;
typedef std::array<int32_t, 2> vec;

template <typename T>
double AxisSquared(T&& x) {
    return (x[0] - x[1]) * (x[0] - x[1]);
}

template <typename T>
double Distance(T&& x, T&& y) {
    return std::sqrt(AxisSquared(x) + AxisSquared(y));
}

std::array<arrayDbl, noCities> CalculateDistanceMatrix(arrayInt x, arrayInt y) {
    std::array<arrayDbl, noCities> distanceMatrix = {};
    for (std::size_t i = 0; i < noCities; i++) {
        for (std::size_t j = 0; j < noCities; j++) {
            distanceMatrix[i][j] = Distance(vec{x[i], x[j]}, vec{y[i], y[j]});
        }
    }
    return distanceMatrix;
}

template<typename InputIterator, typename OutputIterator, typename Predicate>
void CopyIfOutput(InputIterator first, InputIterator last,
                  OutputIterator result, Predicate predicate) {
    for (; first != last; ++first, ++result) {
        if (predicate(*result)) {
            *result = *first;
        }
    }
}

template<typename T>
T CrossoverParents(const T& parent1, const T& parent2) {
    T offspring = {};
    std::size_t index = 0, i = 0;
    typename T::const_iterator iter;
    using type = typename T::value_type;
    while (i++ < parent1.size()) {
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


template <typename popType, typename T>
void CalculateCostValue(popType population, std::array<arrayDbl, noCities> distanceMatrix, T& calculatedCostValue){
    std::size_t parentIndex = 0;
    for (auto& parent : population) {
        calculatedCostValue[parentIndex] += distanceMatrix[parent.front()][parent.back()];
        for (std::size_t i = 0; i < noCities - 1; i++) {
            calculatedCostValue[parentIndex] += distanceMatrix[parent[i]][parent[i + 1]];
        }
        parentIndex++;
    }
}

//TODO: random init population template<class T, std::size_t size>

int main() {
    constexpr std::size_t noParents = 250;
    constexpr double n = 0.8; // need a better name for this variable
    const double pm = 0.2; // need a better name for this variable
    const std::size_t tMax = 1000;

    arrayInt x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    arrayInt y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    arrayInt cities;
    std::iota(cities.begin(), cities.end(), 0);
    std::cout << cities << std::endl << x << std::endl << y << std::endl;

    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    std::array<arrayInt, noParents> population = {};
    for (auto& parent : population) {
        parent = cities;
        std::shuffle(parent.begin(), parent.end(), randomNumberGenerator);
        std::cout << parent << std::endl;
    }

    auto distanceMatrix = CalculateDistanceMatrix(x, y);

    for (auto& t : distanceMatrix) {
        std::cout << t << std::endl;
    }

    std::array<double, noParents> parentCostValues = {};
    CalculateCostValue(population, distanceMatrix, parentCostValues);
//    for (const auto& cost : parentCostValues) {
//        std::cout << "Cost value is: " << cost << std::endl;
//    }

    auto maxCost = std::max_element(parentCostValues.begin(), parentCostValues.end());
    std::cout << "max val is: " << *maxCost << std::endl;
    double ts = 0;
    for (auto& cost : parentCostValues){
        ts += *maxCost - cost;
    }
    std::cout << "ts is: " << ts << std::endl;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> realDistribution(0.0, ts);
    const std::size_t noChosenParents = n * noParents;
    std::array<arrayInt, noChosenParents> chosenParents = {};

    double ti = 0;
    for (auto& chosenParent : chosenParents) {
        double randomNumber = realDistribution(rng);
        for (std::size_t i = 0; i < noParents; i++) {
            ti += *maxCost - parentCostValues[i];
            if (ti >= randomNumber) {
                chosenParent = population[i];
                ti = 0;
                break;
            }
        }
    }

    for (const auto& parent : chosenParents)
        std::cout << parent << std::endl;

//    std::array<int32_t, 5> parent1 = {2, 4, 5, 1, 3};
//    std::array<int32_t, 5> parent2 = {1, 5, 4, 2, 3};
//
//    auto offspring1 = CrossoverParents(parent1, parent2);
//    auto offspring2 = CrossoverParents(parent2, parent1);
//
//    std::cout << offspring1 << " " << offspring2 << std::endl;
    std::array<arrayInt, noChosenParents> offspring = {};
    std::uniform_int_distribution<> intDistribution(0, 199);
    std::size_t indexOfParent_1 = 0;
    std::size_t indexOfParent_2 = 0;
    for (std::size_t i = 0; i < noChosenParents; i += 2) {
        indexOfParent_1 = intDistribution(rng);
        indexOfParent_2 = intDistribution(rng);
        offspring[i] = CrossoverParents(chosenParents[indexOfParent_1], chosenParents[indexOfParent_2]);
        offspring[i + 1] = CrossoverParents(chosenParents[indexOfParent_2], chosenParents[indexOfParent_1]);
    }
    for (auto& child : offspring){
        std::cout << "OFFSPRING:" << child << std::endl;
    }

    double mutationProbability = 0.2;
    std::uniform_real_distribution<> probabilityDistribution(0.0, 1.0);
    std::uniform_int_distribution<> mutationDistribution(0, 9);
    std::size_t mutatingGen_1 = 0;
    std::size_t mutatingGen_2 = 0;
    std::size_t tmp = 0;
    std::size_t mutationCount = 0;
    double mutationChance = 0.0;
    for (auto& child : offspring){
        mutationChance = probabilityDistribution(rng);
        if (mutationChance < mutationProbability) {
            std::cout << "BEFORE MUTATION: " << child << std::endl;
            mutatingGen_1 = mutationDistribution(rng);
            mutatingGen_2 = mutationDistribution(rng);
            tmp = child[mutatingGen_1];
            child[mutatingGen_1] = child[mutatingGen_2];
            child[mutatingGen_2] = tmp;
            std::cout << "AFTER MUTATION:  " << child << std::endl;
            mutationCount++;
        }
    }
    std::cout << "Number of mutated: " << mutationCount << std::endl;

    std::array<double, noChosenParents> offspringCostValues = {};
    CalculateCostValue(population, distanceMatrix, offspringCostValues);
    for (const auto& cost : offspringCostValues) {
        std::cout << "Cost value is: " << cost << std::endl;
    }

    std::array<arrayInt, noParents> newPopulation = population;
    int noChangedCasesDebug=0;
    for (std::size_t i=0; i<noChosenParents; i++){
        auto maxCostPosition = std::distance(parentCostValues.begin(),
                                             std::max_element(parentCostValues.begin(), parentCostValues.end()));
        if(offspringCostValues[i]<parentCostValues[maxCostPosition]){
            newPopulation[maxCostPosition]=offspring[i];
            parentCostValues[maxCostPosition]=offspringCostValues[i];
            noChangedCasesDebug++;
        }
    }
    std::cout << noChangedCasesDebug << std::endl;
    population=newPopulation;
    /*teraz tylko zapetlic i algorytm gotowy kappa*/
    return 0;
}
