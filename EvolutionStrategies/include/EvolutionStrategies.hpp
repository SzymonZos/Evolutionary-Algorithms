#ifndef EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
#define EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP

#include <utility>

#include "Chromosome.hpp"
#include "Distributions.hpp"
#include "Model.hpp"

namespace ES {

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
                        std::size_t offspringFactor) :
        model_{std::move(model)},
        noParents_{model_.front().size()},
        noOffspring_{offspringFactor * noParents_},
        parents_{noParents_},
        offspring_{noOffspring_},
        distributions_{noParents_} {
        Run();
    }

    void Run() {
        InitializePopulation();
        auto parentsEvaluation = Evaluate(parents_);
        CreateOffspring();
        auto offspringEvaluation = Evaluate(offspring_);
    }

private:
    static constexpr double n_ = 3.0;
    static constexpr double tau1_ = 1.0 / CtSqrt(2.0 * n_);
    static constexpr double tau2_ = 1.0 / CtSqrt(2.0 * CtSqrt(n_));

    std::array<std::vector<double>, 2> model_;
    std::size_t noParents_;
    std::size_t noOffspring_;
    std::vector<Chromosome<noCoefficients>> parents_;
    std::vector<Chromosome<noCoefficients>> offspring_;

    std::random_device randomDevice_{};
    std::mt19937 rng_{randomDevice_()};
    Distributions distributions_;

    void InitializePopulation() {
        std::generate(parents_.begin(), parents_.end(), [&] {
            ES::Chromosome<noCoefficients> random;
            std::generate(random[coeffs].begin(), random[coeffs].end(), [&] {
                return distributions_.coefficients(rng_);
            });
            std::generate(random[stddevs].begin(), random[stddevs].end(), [&] {
                return distributions_.standardDeviations(rng_);
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
            return MeanSquaredError(model_, population[idx++]);
        });
        return evaluation;
    }

    void CreateOffspring() {
        std::generate(offspring_.begin(), offspring_.end(), [&] {
            return parents_[parentsDistribution(rng_)];
        });
        auto MutateChild = std::bind(&EvolutionStrategies::MutateChild,
                                     this,
                                     std::placeholders::_1);
        std::for_each(offspring_.begin(), offspring_.end(), MutateChild);
    }
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_EVOLUTIONSTRATEGIES_HPP
