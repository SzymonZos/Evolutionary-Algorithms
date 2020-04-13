#ifndef EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
#define EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP

#include "Chromosome.hpp"
#include "Distributions.hpp"
#include "Model.hpp"
#include "Population.hpp"
#include "StopConditions.hpp"
#include <tuple>

namespace ES {

template<std::size_t noCoefficients>
class EvolutionStrategies {
public:
    using GivenChromosome = Chromosome<noCoefficients>;

    EvolutionStrategies() = delete;
    ~EvolutionStrategies() = default;

    EvolutionStrategies(const EvolutionStrategies& other) = delete;
    EvolutionStrategies& operator=(const EvolutionStrategies& other) = delete;

    EvolutionStrategies(EvolutionStrategies&& other) = delete;
    EvolutionStrategies& operator=(EvolutionStrategies&& other) = delete;

    explicit EvolutionStrategies(Model::Type model,
                                 PopulationSize populationSize = defaultPop,
                                 StopConditions stopConditions = defaultStop) :
        model_{std::move(model)},
        population_{populationSize},
        stopConditions_{stopConditions},
        distributions_{population_.noParents} {
        Run();
    }

    void Run() {
        InitializePopulation();
        do {
            stopConditions_.iteration++;
            population_.parentsEvaluation = Evaluate(population_.parents);
            CreateOffspring();
            population_.offspringEvaluation = Evaluate(population_.offspring);
            ReplacePopulation();
            std::cout << stopConditions_.mse << std::endl;
        } while (stopConditions_.notAchieved());
    }

    std::tuple<GivenChromosome, double, std::size_t> GetResults() const {
        return {population_.parents[0],
                stopConditions_.mse,
                stopConditions_.iteration};
    }

private:
    static constexpr double n_ = noCoefficients;
    static constexpr double tau1_ = 1.0 / CtSqrt(2.0 * n_);
    static constexpr double tau2_ = 1.0 / CtSqrt(2.0 * CtSqrt(n_));

    Model::Type model_;
    Population<noCoefficients> population_;
    StopConditions stopConditions_;

    std::random_device randomDevice_{};
    std::mt19937 rng_{randomDevice_()};
    Distributions distributions_;

    void InitializePopulation() {
        auto& parents = population_.parents;

        std::generate(parents.begin(), parents.end(), [&] {
            GivenChromosome random;
            std::generate(random[coeffs].begin(), random[coeffs].end(), [&] {
                return distributions_.coefficient(rng_);
            });
            std::generate(random[stddevs].begin(), random[stddevs].end(), [&] {
                return distributions_.standardDeviation(rng_);
            });
            return random;
        });
    }

    void MutateChild(GivenChromosome& child) {
        using params = Distributions::params;
        GivenChromosome random{};
        std::size_t i = 0;

        std::generate(random[coeffs].begin(), random[coeffs].end(), [&] {
            return distributions_.child(rng_, params{0, child[stddevs][i++]});
        });
        std::generate(random[stddevs].begin(), random[stddevs].end(), [&] {
            return std::exp(tau1_ * distributions_.standard(rng_)) *
                   std::exp(tau2_ * distributions_.standard(rng_));
        });
        child += random;
    }

    double MeanSquaredError(const GivenChromosome& chromosome) const {
        double meanSquaredError = 0.0;

        for (std::size_t i = 0; i < model_.front().size(); i++) {
            double error = chromosome(model_[in][i]) - model_[out][i];
            meanSquaredError += (error * error);
        }
        return meanSquaredError / static_cast<double>(model_.front().size());
    }

    auto Evaluate(const std::vector<GivenChromosome>& population) const {
        std::vector<double> evaluation(population.size());
        std::size_t idx = 0;

        std::generate(evaluation.begin(), evaluation.end(), [&] {
            return MeanSquaredError(population[idx++]);
        });
        return evaluation;
    }

    void CreateOffspring() {
        auto& offspring = population_.offspring;
        auto MutateChild = std::bind(&EvolutionStrategies::MutateChild,
                                     this,
                                     std::placeholders::_1);

        std::generate(offspring.begin(), offspring.end(), [&] {
            return population_.parents[distributions_.parent(rng_)];
        });
        std::for_each(offspring.begin(), offspring.end(), MutateChild);
    }

    void SortPopulation() {
        auto& offspring = population_.offspring;
        auto& offspringEval = population_.offspringEvaluation;
        auto& sorted = population_.sorted;

        for (std::size_t i = 0; i < population_.noOffspring; i++) {
            sorted.insert({offspringEval[i], offspring[i]});
            switch (population_.strategyType) {
            case StrategyType::parentsAndOffspring:
                if (i < population_.noParents) {
                    sorted.insert({population_.parentsEvaluation[i],
                                   population_.parents[i]});
                }
                break;
            case StrategyType::offspring:
                break;
            }
        }
    }

    void ReplacePopulation() {
        auto& parents = population_.parents;

        SortPopulation();
        auto it = population_.sorted.begin();
        std::generate(parents.begin(), parents.end(), [&] {
            return (it++)->second;
        });
        if (population_.sorted.begin()->first == stopConditions_.mse) {
            stopConditions_.noConsecutiveSameMse++;
        } else {
            stopConditions_.noConsecutiveSameMse = 0;
        }
        stopConditions_.mse = population_.sorted.begin()->first;
        population_.sorted.clear();
    }
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
