#ifndef TRAVELLINGSALESMANPROBLEM_MATH_HPP
#define TRAVELLINGSALESMANPROBLEM_MATH_HPP

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

template<typename T>
constexpr double AxisSquared(T&& x1, T&& x2) {
    return (x1 - x2) * (x1 - x2);
}

template<typename T>
constexpr double Distance(T&& x1, T&& x2, T&& y1, T&& y2) {
    return CtSqrt(AxisSquared(x1, x2) + AxisSquared(y1, y2));
}

#endif //TRAVELLINGSALESMANPROBLEM_MATH_HPP
