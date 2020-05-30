#ifndef ANTSYSTEMS_ANTSYSTEM_HPP
#define ANTSYSTEMS_ANTSYSTEM_HPP

#include "GenericOstreamOperators.hpp"
#include "Math.hpp"
#include "Types.hpp"
#include <iostream>
#include <map>
#include <random>
#include <tuple>

template<std::size_t noAnts>
class AntSystem {
public:
    AntSystem(std::size_t tMax,
              const DblMatrix<noAnts, noAnts>& distanceMatrix) :
        tMax_{tMax},
        distanceMatrix_{distanceMatrix} {
        Run();
    }

    AntSystem() = delete;
    ~AntSystem() = default;

    void Run() {
        CreateInitialPheromoneTrace();
        for (std::size_t i = 0; i < tMax_; i++) {
            for (auto& tour : tours_) {
                for (auto& t : tour) {
                    t = 0;
                }
            }
            for (auto& tour : visitedNodes_) {
                for (auto& t : tour) {
                    t = false;
                }
            }
            for (std::size_t m = 0; m < noAnts; m++) {
                visitedNodes_[m][m] = true;
                visitedCities_[m][0] = m;
                for (std::size_t j = 0; j < noAnts - 1; j++) {
                    double notVisited{};
                    DblArray<noAnts> a{};
                    std::vector<double> p;
                    auto currentCity = j ? visitedCities_[m][j - 1] : m;
                    for (std::size_t k = 0; k < noAnts; k++) {
                        if (currentCity != k && !visitedNodes_[m][k]) {
                            notVisited += std::pow(
                                              pheromoneQuantity_[currentCity]
                                                                [k],
                                              alpha_) *
                                          std::pow(
                                              1 / distanceMatrix_[currentCity]
                                                                 [k],
                                              beta_);
                        }
                    }
                    for (std::size_t k = 0; k < noAnts; k++) {
                        if (currentCity != k && !visitedNodes_[m][k]) {
                            a[k] = std::pow(pheromoneQuantity_[currentCity][k],
                                            alpha_) *
                                   std::pow(
                                       1 / distanceMatrix_[currentCity][k],
                                       beta_) /
                                   notVisited;
                        }
                    }
                    auto sumA = std::accumulate(a.begin(), a.end(), 0.0);
                    for (std::size_t k = 0; k < noAnts; k++) {
                        p.push_back(a[k] / sumA);
                    }
                    for (std::size_t k = 1; k < p.size(); k++) {
                        p[k] += p[k - 1];
                    }
                    // SelectNode();
                    auto randomNumber = probabilityDist_(rng_);
                    std::size_t visited{};
                    while (p[visited] < randomNumber) {
                        visited++;
                    }
                    visitedNodes_[m][visited] = true;
                    visitedCities_[m][j + 1] = visited;
                    tours_[currentCity]
                          [visited] += distanceMatrix_[currentCity][visited];
                    tours_[visited]
                          [currentCity] += distanceMatrix_[currentCity]
                                                          [visited];
                }
                std::size_t dupa{};
                for (auto& quantity : pheromoneQuantity_) {
                    quantity *= (1 - evaporationCoeff_);
                    quantity += tours_[dupa++];
                }
            }
        }
        auto costValues = CalculateCostValues(visitedCities_);
        auto index = std::min_element(costValues.begin(), costValues.end());
        minCostValue_ = *index;
        minCostIndex_ = static_cast<std::size_t>(
            std::distance(costValues.begin(), index));
    }

    std::tuple<IntArray<noAnts>, double> GetResult() {
        return {visitedCities_[minCostIndex_], minCostValue_};
    }

private:
    std::size_t tMax_;
    DblMatrix<noAnts, noAnts> distanceMatrix_;

    std::mt19937 rng_{std::random_device{}()};
    std::uniform_real_distribution<> probabilityDist_{0.0, 1.0};

    static const std::size_t alpha_{1};
    static const std::size_t beta_{5};
    constexpr static const double evaporationCoeff_{0.5};

    DblMatrix<noAnts, noAnts> pheromoneQuantity_{};
    std::array<std::array<bool, noAnts>, noAnts> visitedNodes_{};
    IntMatrix<noAnts, noAnts> visitedCities_{};
    DblMatrix<noAnts, noAnts> probabilities_{};
    DblMatrix<noAnts, noAnts> tours_{};
    double minCostValue_{};
    std::size_t minCostIndex_{};

    void CreateInitialPheromoneTrace() {
        const double factor{0.2};
        for (std::size_t i = 0; i < noAnts; i++) {
            for (std::size_t j = 0; j < noAnts; j++) {
                if (i != j) {
                    pheromoneQuantity_[j][i] = factor * probabilityDist_(rng_);
                }
            }
        }
    }

    void SelectNode() {
        // roulette wheel
    }

    auto CalculateCostValues(const IntMatrix<noAnts, noAnts>& population) {
        DblArray<noAnts> costValues = {};
        std::size_t idx = 0;
        for (auto& chromosome : population) {
            costValues[idx] += distanceMatrix_[chromosome.front()]
                                              [chromosome.back()];
            for (std::size_t i = 0; i < chromosome.size() - 1; i++) {
                costValues[idx] += distanceMatrix_[chromosome[i]]
                                                  [chromosome[i + 1]];
            }
            idx++;
        }
        return costValues;
    }
};

#endif // ANTSYSTEMS_ANTSYSTEM_HPP
