#ifndef EVOLUTIONSTRATEGIES_TYPES_HPP
#define EVOLUTIONSTRATEGIES_TYPES_HPP

#include <array>
#include <functional>

template<typename BinaryOperation, typename T, std::size_t size>
auto ElementWise(const std::array<T, size>& lhs,
                 const std::array<T, size>& rhs,
                 BinaryOperation operation) {
    std::array<T, size> ret;
    for (std::size_t i = 0; i < size; i++) {
        ret[i] = std::invoke(operation, lhs[i], rhs[i]);
    }
    return ret;
}

template<typename T, std::size_t size>
auto operator+(const std::array<T, size>& lhs,
               const std::array<T, size>& rhs) {
    return ElementWise(lhs, rhs, std::plus<T>());
}

template<typename T, std::size_t size>
auto& operator+=(std::array<T, size>& lhs, const std::array<T, size>& rhs) {
    return lhs = lhs + rhs;
}

template<typename T, std::size_t size>
auto operator-(const std::array<T, size>& lhs,
               const std::array<T, size>& rhs) {
    return ElementWise(lhs, rhs, std::minus<T>());
}

template<typename T, std::size_t size>
auto& operator-=(std::array<T, size>& lhs, const std::array<T, size>& rhs) {
    return lhs = lhs - rhs;
}

template<typename T, std::size_t size>
auto operator*(const std::array<T, size>& lhs,
               const std::array<T, size>& rhs) {
    return ElementWise(lhs, rhs, std::multiplies<T>());
}

template<typename T, std::size_t size>
auto& operator*=(std::array<T, size>& lhs, const std::array<T, size>& rhs) {
    return lhs = lhs * rhs;
}

template<typename T, std::size_t size>
auto operator/(const std::array<T, size>& lhs,
               const std::array<T, size>& rhs) {
    return ElementWise(lhs, rhs, std::divides<T>());
}

template<typename T, std::size_t size>
auto& operator/=(std::array<T, size>& lhs, const std::array<T, size>& rhs) {
    return lhs = lhs / rhs;
}

template<std::size_t size>
using IntArray = std::array<std::size_t, size>;

template<std::size_t size>
using DblArray = std::array<double, size>;

template<std::size_t inner, std::size_t outer>
using IntMatrix = std::array<IntArray<inner>, outer>;

template<std::size_t inner, std::size_t outer>
using DblMatrix = std::array<DblArray<inner>, outer>;

#endif // EVOLUTIONSTRATEGIES_TYPES_HPP
