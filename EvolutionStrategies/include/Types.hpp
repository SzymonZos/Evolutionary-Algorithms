#ifndef EVOLUTIONSTRATEGIES_TYPES_HPP
#define EVOLUTIONSTRATEGIES_TYPES_HPP

#include <array>

template<std::size_t size>
using IntArray = std::array<std::size_t, size>;

template<std::size_t size>
using DblArray = std::array<double, size>;

template<std::size_t inner, std::size_t outer>
using IntMatrix = std::array<IntArray<inner>, outer>;

template<std::size_t inner, std::size_t outer>
using DblMatrix = std::array<DblArray<inner>, outer>;

#endif // EVOLUTIONSTRATEGIES_TYPES_HPP
