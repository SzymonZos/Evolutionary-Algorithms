#ifndef ANTSYSTEMS_ANTCOLONY_HPP
#define ANTSYSTEMS_ANTCOLONY_HPP

#include "GenericOstreamOperators.hpp"
#include "Math.hpp"
#include "Types.hpp"
#include <iostream>
#include <map>
#include <random>
#include <tuple>

template<std::size_t noAnts>
class AntColony {
public:
    AntColony(std::size_t tMax, const DblArray<2>& distanceMatrix) :
        tMax_{tMax},
        distanceMatrix_{distanceMatrix} {
        Run();
    }

    AntColony() = delete;
    ~AntColony() = default;

    void Run() {
        for (std::size_t i = 0; i < tMax_; i++) {
            for (std::size_t m = 0; m < noAnts; m++) {
                for (std::size_t j = 0; j < 2; j++) {
                    auto denominator = std::pow(antsCounter_[j][0] + k_, d_) +
                                       std::pow(antsCounter_[j][1] + k_, d_);
                    auto pr = std::pow(antsCounter_[j][1] + k_, d_) /
                              (denominator * distanceMatrix_[1]);
                    auto pl = std::pow(antsCounter_[j][0] + k_, d_) /
                              (denominator * distanceMatrix_[0]);
                    pr /= (pl + pr);
                    if (probabilityDist_(rng_) <= pr) {
                        antsCounter_[j][1]++;
                    } else {
                        antsCounter_[j][0]++;
                    }
                }
            }
        }
    }

    [[nodiscard]] IntMatrix<2, 2> GetResult() const {
        return antsCounter_;
    }

private:
    std::size_t tMax_;
    DblArray<2> distanceMatrix_;

    std::mt19937 rng_{std::random_device{}()};
    std::uniform_real_distribution<> probabilityDist_{0.0, 1.0};

    static const std::size_t k_{20};
    static const std::size_t d_{2};

    IntMatrix<2, 2> antsCounter_{};
};

#endif // ANTSYSTEMS_ANTCOLONY_HPP
