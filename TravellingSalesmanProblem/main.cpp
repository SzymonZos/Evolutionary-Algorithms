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
double AxisSquared(const T& x) {
    return (x[0] - x[1]) * (x[0] - x[1]);
}

template <typename T>
double Distance(const T& x, const T& y) {
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

    std::array<double, noParents> costValues = {};
    std::size_t parentIndex = 0;

    for (auto& parent : population) {
        costValues[parentIndex] += distanceMatrix[parent.front()][parent.back()];
        for (std::size_t i = 0; i < noCities - 1; i++) {
            costValues[parentIndex] += distanceMatrix[parent[i]][parent[i + 1]];
        }
        parentIndex++;
    }

//    for (const auto& cost : costValues) {
//        std::cout << "Cost value is: " << cost << std::endl;
//    }

    auto maxCost = std::max_element(costValues.begin(), costValues.end());
    std::cout << "max val is: " << *maxCost << std::endl;
    double ts = 0;
    for (auto& cost : costValues){
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
            ti += *maxCost - costValues[i];
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
    std::array<arrayInt, noParents> offspring = {};
    std::uniform_int_distribution<> intDistribution(0, 199);
    std::size_t indexOfParent_1 = 0;
    std::size_t indexOfParent_2 = 0;
    for (auto& child : offspring){
        indexOfParent_1 = intDistribution(rng);
        indexOfParent_2 = intDistribution(rng);
        child = CrossoverParents(chosenParents[indexOfParent_1],chosenParents[indexOfParent_2]);
    }
    for (auto child : offspring){
        std::cout << "OFFSPRING:" << child << std::endl;
    }
    return 0;
}
