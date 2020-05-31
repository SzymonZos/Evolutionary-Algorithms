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
        for (std::size_t t = 0; t < tMax_; t++) {
            TraverseNetwork();
            CalculateDistance();
            EvaluateTours();
            UpdatePheromone();
            UpdateBestTour();
            ClearParameters();
        }
    }

    std::tuple<IntArray<noAnts>, double> GetResult() const {
        return {bestRoute_, minCostValue_};
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
    std::vector<double> probabilities_{};
    DblMatrix<noAnts, noAnts> tours_{};
    DblArray<noAnts> distances_{};

    double minCostValue_{std::numeric_limits<double>::max()};
    IntArray<noAnts> bestRoute_{};

    void CreateInitialPheromoneTrace() {
        for (std::size_t i = 0; i < noAnts; i++) {
            for (std::size_t j = 0; j < noAnts; j++) {
                if (i != j) {
                    pheromoneQuantity_[i][j] = 1 / distanceMatrix_[i][j];
                }
            }
        }
    }

    void ClearParameters() {
        for (auto& tour : tours_) {
            tour = {};
        }
        for (auto& node : visitedNodes_) {
            node = {};
        }
        distances_ = {};
    }

    auto CalculateDecisionTable(const std::size_t ant,
                                const std::size_t curr) {
        DblArray<noAnts> a{};
        double notVisited{};
        for (std::size_t i = 0; i < noAnts; i++) {
            if (!visitedNodes_[ant][i]) {
                notVisited += std::pow(pheromoneQuantity_[curr][i], alpha_) *
                              std::pow(1 / distanceMatrix_[curr][i], beta_);
            }
        }
        for (std::size_t i = 0; i < noAnts; i++) {
            if (!visitedNodes_[ant][i]) {
                a[i] = std::pow(pheromoneQuantity_[curr][i], alpha_) *
                       std::pow(1 / distanceMatrix_[curr][i], beta_) /
                       notVisited;
            }
        }
        return a;
    }

    auto CalculateProbabilities(const DblArray<noAnts>& decisionTable) {
        const auto decisionSum = std::accumulate(decisionTable.cbegin(),
                                                 decisionTable.cend(),
                                                 0.0);
        for (std::size_t k = 0; k < noAnts; k++) {
            probabilities_.push_back(decisionTable[k] / decisionSum);
        }
        for (std::size_t k = 1; k < probabilities_.size(); k++) {
            probabilities_[k] += probabilities_[k - 1];
        }
    }

    void SelectNode(std::size_t ant, std::size_t nextNode) {
        const auto randomNumber = probabilityDist_(rng_);
        const auto index = static_cast<std::size_t>(
            std::distance(probabilities_.cbegin(),
                          std::upper_bound(probabilities_.cbegin(),
                                           probabilities_.cend(),
                                           randomNumber)));
        visitedNodes_[ant][index] = true;
        visitedCities_[ant][nextNode] = index;
    }

    void TraverseNetwork() {
        for (std::size_t ant = 0; ant < noAnts; ant++) {
            visitedNodes_[ant][ant] = true;
            visitedCities_[ant][0] = ant;
            for (std::size_t j = 0; j < noAnts - 1; j++) {
                const auto currentNode = visitedCities_[ant][j];
                const auto table = CalculateDecisionTable(ant, currentNode);
                CalculateProbabilities(table);
                SelectNode(ant, j + 1);
                probabilities_.clear();
            }
        }
    }

    void CalculateDistance() {
        std::size_t idx = 0;
        for (auto& city : visitedCities_) {
            distances_[idx] += distanceMatrix_[city.front()][city.back()];
            for (std::size_t i = 0; i < city.size() - 1; i++) {
                distances_[idx] += distanceMatrix_[city[i]][city[i + 1]];
            }
            idx++;
        }
    }

    void EvaluateTours() {
        for (std::size_t i = 0; i < noAnts; i++) {
            for (std::size_t j = 0; j < noAnts - 1; j++) {
                tours_[visitedCities_[i][j]]
                      [visitedCities_[i][j + 1]] += 1 / distances_[i];
                tours_[visitedCities_[i][j + 1]]
                      [visitedCities_[i][j]] += 1 / distances_[i];
            }
            tours_[visitedCities_[i].front()]
                  [visitedCities_[i].back()] += 1 / distances_[i];
            tours_[visitedCities_[i].back()]
                  [visitedCities_[i].front()] += 1 / distances_[i];
        }
    }

    void UpdatePheromone() {
        std::size_t i{};
        for (auto& quantity : pheromoneQuantity_) {
            quantity *= (1 - evaporationCoeff_);
            quantity += tours_[i++];
        }
    }

    void UpdateBestTour() {
        auto index = std::min_element(distances_.begin(), distances_.end());
        if (*index < minCostValue_) {
            minCostValue_ = *index;
            bestRoute_ = visitedCities_[static_cast<std::size_t>(
                std::distance(distances_.begin(), index))];
        }
    }
};

#endif // ANTSYSTEMS_ANTSYSTEM_HPP
