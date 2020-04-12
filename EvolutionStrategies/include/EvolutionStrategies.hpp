#ifndef EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
#define EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP

#include <utility>

#include "Chromosome.hpp"
#include "Distributions.hpp"
#include "Model.hpp"
#include "Population.hpp"

namespace ES {
enum class StrategyType : std::size_t { parentsAndOffspring, offspring };

template<std::size_t noCoefficients>
class EvolutionStrategies {
public:
    EvolutionStrategies() = delete;
    ~EvolutionStrategies() = default;

    EvolutionStrategies(const EvolutionStrategies& other) = delete;
    EvolutionStrategies& operator=(const EvolutionStrategies& other) = delete;

    EvolutionStrategies(EvolutionStrategies&& other) = delete;
    EvolutionStrategies& operator=(EvolutionStrategies&& other) = delete;

    EvolutionStrategies(std::array<std::vector<double>, 2> model,
                        std::size_t offspringFactor,
                        StrategyType strategyType) :
        model_{std::move(model)},
        population_{model_.front().size(), offspringFactor},
        strategyType_{strategyType},
        distributions_{population_.noParents} {
        Run();
    }

    void Run() {
        InitializePopulation();
        auto parentsEvaluation = Evaluate(population_.parents);
        CreateOffspring();
        auto offspringEvaluation = Evaluate(population_.offspring);
    }

private:
    static constexpr double n_ = noCoefficients;
    static constexpr double tau1_ = 1.0 / CtSqrt(2.0 * n_);
    static constexpr double tau2_ = 1.0 / CtSqrt(2.0 * CtSqrt(n_));

    std::array<std::vector<double>, 2> model_;
    Population<noCoefficients> population_;
    StrategyType strategyType_;

    std::random_device randomDevice_{};
    std::mt19937 rng_{randomDevice_()};
    Distributions distributions_;

    void InitializePopulation() {
        auto& parents = population_.parents;
        std::generate(parents.begin(), parents.end(), [&] {
            ES::Chromosome<noCoefficients> random;
            std::generate(random[coeffs].begin(), random[coeffs].end(), [&] {
                return distributions_.coefficient(rng_);
            });
            std::generate(random[stddevs].begin(), random[stddevs].end(), [&] {
                return distributions_.standardDeviation(rng_);
            });
            return random;
        });
    }

    void MutateChild(Chromosome<noCoefficients>& child) {
        using params = Distributions::params;
        Chromosome<noCoefficients> random{};
        std::size_t i = 0;
        std::generate(random[coeffs].begin(), random[coeffs].end(), [&] {
            return distributions_.child(rng_, params{0, child[stddevs][i++]});
        });
        std::generate(random[stddevs].begin(), random[stddevs].end(), [&] {
            return std::exp(tau1_ * distributions_.standard(rng_)) +
                   std::exp(tau2_ * distributions_.standard(rng_));
        });
        child += random;
    }

    double MeanSquaredError(const Chromosome<noCoefficients>& chromosome) {
        double meanSquaredError = 0.0;
        for (std::size_t i = 0; i < population_.noParents; i++) {
            double error = chromosome(model_[in][i]) - model_[out][i];
            meanSquaredError += (error * error);
        }
        return meanSquaredError / static_cast<double>(population_.noParents);
    }

    auto Evaluate(const std::vector<Chromosome<noCoefficients>>& population) {
        std::vector<double> evaluation(population.size());
        std::size_t idx = 0;
        std::generate(evaluation.begin(), evaluation.end(), [&] {
            return MeanSquaredError(population[idx++]);
        });
        return evaluation;
    }

    void CreateOffspring() {
        auto& offspring = population_.offspring;
        std::generate(offspring.begin(), offspring.end(), [&] {
            return population_.parents[distributions_.parent(rng_)];
        });
        auto MutateChild = std::bind(&EvolutionStrategies::MutateChild,
                                     this,
                                     std::placeholders::_1);
        std::for_each(offspring.begin(), offspring.end(), MutateChild);
    }

    void SortPopulation() {
        for (std::size_t i = 0; i < population_.noOffspring; i++) {
            population_.sorted.insert(
                {population_.childrenEvaluation[i], population_.children[i]});
            if (i < population_.noParents) {
                sortedPopulation.insert({population_.parentsEvaluation[i],
                                         population_.parents[i]});
            }
        }
    }

    void ReplacePopulation() {
        minCostValue_ = sortedPopulation_.begin()->first;
        sortedPopulation_.clear();
    }
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
