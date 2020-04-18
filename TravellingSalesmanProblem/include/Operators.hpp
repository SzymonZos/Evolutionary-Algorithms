#ifndef TRAVELLINGSALESMANPROBLEM_OPERATORS_HPP
#define TRAVELLINGSALESMANPROBLEM_OPERATORS_HPP

#include <array>
#include <iostream>

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& array) {
    stream << "[";
    for (const auto& value : array) {
        stream << value << ", ";
    }
    stream << "\b\b]";
    return stream;
}

template<template<typename...> class TT,
         typename... T,
         typename = std::enable_if_t<!std::is_same_v<TT<T...>, std::string>>>
std::ostream& operator<<(std::ostream& stream, const TT<T...>& collection) {
    stream << "[";
    for (const auto& value : collection) {
        stream << value << ", ";
    }
    stream << "\b\b]";
    return stream;
}

template<typename Map>
void print_map(const Map& m) {
    for (const auto& p : m) {
        std::cout << '\'' << p.first << "': [";
        for (const auto& v : p.second) {
            std::cout << v << ", ";
        }
        std::cout << "\b\b]\n";
    }
}

#endif // TRAVELLINGSALESMANPROBLEM_OPERATORS_HPP
