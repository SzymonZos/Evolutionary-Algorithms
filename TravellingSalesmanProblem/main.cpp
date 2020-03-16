#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <random>

// y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]

// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0]

typedef std::array<uint32_t, 10> array;

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

//TODO: random init population template<class T, std::size_t size>

int main() {
    const std::size_t noParents = 250;
    const double n = 0.8; // need a better name for this variable
    const double pm = 0.2; // need a better name for this variable
    const std::size_t tMax = 1000;

    array x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    array y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    array cities;
    std::iota(cities.begin(), cities.end(), 1);
    std::cout << cities << std::endl << x << std::endl << y << std::endl;

    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    std::array<array, noParents> population = {};
    for (auto& parent : population) {
        parent = cities;
        std::shuffle(parent.begin(), parent.end(), randomNumberGenerator);
        std::cout << parent << std::endl;
    }

    return 0;
}
