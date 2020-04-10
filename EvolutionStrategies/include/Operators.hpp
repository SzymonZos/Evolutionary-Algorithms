#ifndef EVOLUTIONSTRATEGIES_OPERATORS_HPP
#define EVOLUTIONSTRATEGIES_OPERATORS_HPP

#include <array>
#include <iostream>
#include <tuple>
#include <type_traits>

template<template<typename...> class TT, typename... T>
std::ostream& operator<<(std::ostream& stream, const TT<T...>& collection) {
    if (!collection.empty()) {
        if (std::is_scalar_v<std::tuple_element_t<0, std::tuple<T...>>>) {
            stream << "[";
            for (auto value : collection) {
                stream << value << ", ";
            }
            stream << "\b\b]\n";
        } else {
            stream << "{\n";
            for (auto it = collection.begin(); it != collection.end(); it++) {
                if (std::next(it) == collection.end()) {
                    stream << *it;
                } else {
                    stream << *it << "\n";
                }
            }
            stream << "}\n";
        }
    }
    return stream;
}

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& array) {
    if (!array.empty()) {
        if (std::is_scalar_v<T>) {
            stream << "[";
            for (auto value : array) {
                stream << value << ", ";
            }
            stream << "\b\b]\n";
        } else {
            stream << "{\n";
            for (const auto& value : array) {
                if (&value == &array.back()) {
                    stream << value;
                } else {
                    stream << value << "\n";
                }
            }
            stream << "}\n";
        }
    }
    return stream;
}

template<class T1, class T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& pair) {
    stream << "{\n" << pair.first << "\n" << pair.second << "}\n";
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

#endif // EVOLUTIONSTRATEGIES_OPERATORS_HPP
