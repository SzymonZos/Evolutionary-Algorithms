#ifndef ANTSYSTEMS_ANTSYSTEM_HPP
#define ANTSYSTEMS_ANTSYSTEM_HPP

#include "GenericOstreamOperators.hpp"
#include "Math.hpp"
#include "Types.hpp"
#include <iostream>
#include <map>
#include <random>
#include <tuple>

template<std::size_t noOffspring, std::size_t noAlleles, std::size_t noParents>
class AntSystem {
public:
    AntSystem(double mutationProbability,
              std::size_t tMax,
              DblMatrix<noAlleles, noAlleles> distanceMatrix) :
        mutationProbability_{mutationProbability},
        tMax_{tMax},
        distanceMatrix_{distanceMatrix} {
        Run();
    }

    AntSystem() = delete;
    ~AntSystem() = default;

    void Run() {
        CreateInitialPopulation();
        for (std::size_t i = 0; i < tMax_; i++) {
            parentCostValues_ = CalculateCostValues(parents_);
            GenerateOffspring(SelectParents());
            MutateOffspring();
            offspringCostValues_ = CalculateCostValues(offspring_);
            ReplacePopulation();
        }
        std::cout << "Best route is: " << parents_[0]
                  << " with cost value: " << minCostValue_ << std::endl;
    }

    std::tuple<IntArray<noAlleles>, double> GetResult() {
        return {parents_[0], minCostValue_};
    }

private:
    double mutationProbability_;
    std::size_t tMax_;
    DblMatrix<noAlleles, noAlleles> distanceMatrix_;

    std::mt19937 rng_{std::random_device{}()};
    std::uniform_int_distribution<> offspringDist_{0, noOffspring - 1};
    std::uniform_int_distribution<> mutationDist_{0, noAlleles - 1};
    std::uniform_real_distribution<> probabilityDist_{0.0, 1.0};

    IntMatrix<noAlleles, noParents> parents_{};
    DblArray<noParents> parentCostValues_{};
    IntMatrix<noAlleles, noOffspring> offspring_{};
    DblArray<noOffspring> offspringCostValues_{};
    std::multimap<double, IntArray<noAlleles>> sortedPopulation_{};
    double minCostValue_{};

    template<std::size_t size>
    auto CalculateCostValues(const IntMatrix<noAlleles, size>& population) {
        DblArray<size> costValues = {};
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

    void CreateInitialPopulation() {
        IntArray<noAlleles> cities;
        std::iota(cities.begin(), cities.end(), 0);
        for (auto& parent : parents_) {
            parent = cities;
            std::shuffle(parent.begin(), parent.end(), rng_);
        }
    }

    auto SelectParents() {
        IntMatrix<noAlleles, noOffspring> chosenParents = {};
        auto maxCost = std::max_element(parentCostValues_.begin(),
                                        parentCostValues_.end());
        double ts = std::accumulate(
            parentCostValues_.begin(),
            parentCostValues_.end(),
            0.0,
            [&](double prev, double curr) { return prev + *maxCost - curr; });
        std::uniform_real_distribution<> realDistribution(0.0, ts);
        for (auto& chosenParent : chosenParents) {
            double randomNumber = realDistribution(rng_);
            double ti = 0;
            for (std::size_t i = 0; i < noParents; i++) {
                ti += *maxCost - parentCostValues_[i];
                if (ti >= randomNumber) {
                    chosenParent = parents_[i];
                    break;
                }
            }
        }
        return chosenParents;
    }

    template<typename T>
    T CrossoverParents(const T& parent1, const T& parent2) {
        using type = typename T::value_type;
        T offspring = {};
        type dummy = noAlleles;
        std::fill(offspring.begin(), offspring.end(), dummy);
        std::size_t index = 0;
        typename T::const_iterator iter;

        for (std::size_t i = 0; i < parent1.size(); i++) {
            offspring[index] = parent1[index];
            iter = std::find(parent1.cbegin(), parent1.cend(), parent2[index]);
            if (iter == parent1.cbegin()) {
                CopyIfOutput(parent2.cbegin(),
                             parent2.cend(),
                             offspring.begin(),
                             [=](type value) { return value == dummy; });
                break;
            }
            index = static_cast<std::size_t>(
                std::distance(parent1.cbegin(), iter));
        }
        return offspring;
    }

    template<typename T>
    void GenerateOffspring(T&& parents) {
        IntArray<2> index = {};
        for (std::size_t i = 0; i < noOffspring; i += 2) {
            std::generate(index.begin(), index.end(), [&] {
                return offspringDist_(rng_);
            });
            offspring_[i] = CrossoverParents(parents[index[0]],
                                             parents[index[1]]);
            offspring_[i + 1] = CrossoverParents(parents[index[1]],
                                                 parents[index[0]]);
        }
    }

    void MutateOffspring() {
        IntArray<2> index = {};
        for (auto& child : offspring_) {
            if (probabilityDist_(rng_) < mutationProbability_) {
                std::generate(index.begin(), index.end(), [&] {
                    return mutationDist_(rng_);
                });
                std::swap(child[index[0]], child[index[1]]);
            }
        }
    }

    void SortPopulation() {
        for (std::size_t i = 0; i < noParents; i++) {
            sortedPopulation_.insert({parentCostValues_[i], parents_[i]});
            if (i < noOffspring) {
                sortedPopulation_.insert(
                    {offspringCostValues_[i], offspring_[i]});
            }
        }
    }

    void ReplacePopulation() {
        SortPopulation();
        auto it = sortedPopulation_.begin();
        std::generate(parents_.begin(), parents_.end(), [&] {
            return (it++)->second;
        });
        minCostValue_ = sortedPopulation_.begin()->first;
        sortedPopulation_.clear();
    }
};

#endif // ANTSYSTEMS_ANTSYSTEM_HPP
