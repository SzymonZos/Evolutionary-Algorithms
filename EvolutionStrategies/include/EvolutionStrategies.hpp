#ifndef EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
#define EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP

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
        do {
            population_.parentsEvaluation = Evaluate(population_.parents);
            CreateOffspring();
            population_.offspringEvaluation = Evaluate(population_.offspring);
        } while (ReplacePopulation() >= 0.3);
    }

    Chromosome<noCoefficients> GetResult() {
        return population_.parents[0];
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
            return std::exp(tau1_ * distributions_.standard(rng_)) *
                   std::exp(tau2_ * distributions_.standard(rng_));
        });
        child += random;
    }

    double MeanSquaredError(const Chromosome<noCoefficients>& chromosome) {
        double meanSquaredError = 0.0;

        for (std::size_t i = 0; i < model_.front().size(); i++) {
            double error = chromosome(model_[in][i]) - model_[out][i];
            meanSquaredError += (error * error);
        }
        return meanSquaredError / static_cast<double>(model_.front().size());
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
            switch (strategyType_) {
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

    double ReplacePopulation() {
        auto& parents = population_.parents;

        SortPopulation();
        auto it = population_.sorted.begin();
        std::generate(parents.begin(), parents.end(), [&] {
            return (it++)->second;
        });
        double minMse = population_.sorted.begin()->first;
        population_.sorted.clear();
        return minMse;
    }
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
