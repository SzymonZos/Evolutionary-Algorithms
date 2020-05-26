#ifndef TRAVELLINGSALESMANPROBLEM_UTILS_HPP
#define TRAVELLINGSALESMANPROBLEM_UTILS_HPP

#include "Math.hpp"
#include "Types.hpp"

template<std::size_t noAlleles>
constexpr auto CalculateDistanceMatrix(const DblArray<noAlleles>& x,
                                       const DblArray<noAlleles>& y) {
    DblMatrix<noAlleles, noAlleles> distanceMatrix = {};
    for (std::size_t i = 0; i < noAlleles; i++) {
        for (std::size_t j = 0; j < noAlleles; j++) {
            distanceMatrix[i][j] = Distance(x[i], x[j], y[i], y[j]);
        }
    }
    return distanceMatrix;
}

#endif // TRAVELLINGSALESMANPROBLEM_UTILS_HPP
