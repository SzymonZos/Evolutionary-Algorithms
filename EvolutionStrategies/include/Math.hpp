#ifndef EVOLUTIONSTRATEGIES_MATH_HPP
#define EVOLUTIONSTRATEGIES_MATH_HPP

#include <cmath>

template<typename T>
constexpr T SqrtNewtonRaphson(T x, T curr, T prev) {
    if (curr == prev) {
        return curr;
    }
    return SqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
}

template<typename T>
constexpr T CtSqrt(T x) {
    if (x >= 0 && x < std::numeric_limits<T>::infinity()) {
        return SqrtNewtonRaphson<T>(x, x, 0);
    }
    return std::numeric_limits<T>::quiet_NaN();
}

#endif // EVOLUTIONSTRATEGIES_MATH_HPP
