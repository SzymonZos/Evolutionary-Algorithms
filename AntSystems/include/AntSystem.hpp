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
            for (std::size_t j = 0; j < noAnts; j++) {
                double notVisited{};
                DblArray<noAnts> a{};
                std::vector<double> p(noAnts - j - 1);
                for (std::size_t k = 0; k < noAnts; k++) {
                    if (!visitedNodes_[j][k]) {
                        notVisited += std::pow(pheromoneQuantity_[k], alpha_) *
                                      std::pow(1 / distanceMatrix_[j][k],
                                               beta_);
                    }
                }
                for (std::size_t k = 0; k < noAnts; k++) {
                    if (!visitedNodes_[j][k]) {
                        a[k] = std::pow(pheromoneQuantity_[k], alpha_) *
                               std::pow(1 / distanceMatrix_[j][k], beta_) /
                               notVisited;
                    }
                }
                auto sumA = std::accumulate(a.begin(), a.end(), 0.0);
                for (std::size_t k = 0; k < noAnts; k++) {
                    if (!visitedNodes_[j][k]) {
                        p.push_back(a[k] / sumA);
                    }
                }
                for (std::size_t k = 1; k < p.size(); k++) {
                    p[k] += p[k - 1];
                }
                // SelectNode();
                auto randomNumber = probabilityDist_(rng_);
                std::size_t visited{};
                while (p[visited++] < randomNumber) {
                }
                visitedNodes_[j][visited] = true;
                tours_[j][visited] += distanceMatrix_[j][visited];
                tours_[visited][j] += distanceMatrix_[j][visited];
            }
            pheromoneQuantity_ *= (1 - evaporationCoeff_) + tours_;
        }
    }

    std::tuple<IntArray<noAnts>, double> GetResult() {
        return {tours_[0], minCostValue_};
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
    DblMatrix<noAnts, noAnts> probabilities_{};
    DblMatrix<noAnts, noAnts> tours_{};
    double minCostValue_{};

    void CreateInitialPheromoneTrace() {
        const double factor{0.02};
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
};

#endif // ANTSYSTEMS_ANTSYSTEM_HPP
