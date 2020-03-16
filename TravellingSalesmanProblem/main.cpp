#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>

// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]
// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& stream, std::array<T, N>& array) {
    stream << "[";
    for (const auto& value : array) {
        stream << value << ", ";
    }
    stream << "\b\b]";
    return stream;
}

template<template<typename...> typename TT, typename... T>
std::ostream& operator<<(std::ostream& stream, const TT<T...>& collection) {
    stream << "[";
    for (const auto& value : collection) {
        stream << value << ", ";
    }
    stream << "\b\b]";
    return stream;
}

typedef std::array<uint32_t, 10> array;

int main() {
    std::size_t noParents = 250;
    double n = 0.8; // need a better name for this variable
    double pm = 0.2; // need a better name for this variable
    std::size_t tMax = 1000;

    array cities;
    std::iota(cities.begin(), cities.end(), 1);
    std::cout << cities;

    return 0;
}
