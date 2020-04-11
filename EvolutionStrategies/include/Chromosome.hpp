#ifndef EVOLUTIONARYALGORITHMS_CHROMOSOME_HPP
#define EVOLUTIONARYALGORITHMS_CHROMOSOME_HPP

#include "Operators.hpp"
#include "Types.hpp"
#include <cmath>
#include <initializer_list>
#include <tuple>

namespace ES {
enum Model : std::size_t { in, out };
enum Coefficient : std::size_t { a, b, c };
enum ChromosomeVector : std::size_t { coeffs, stddevs };

template<std::size_t size>
class Chromosome {
public:
    Chromosome() = default;
    ~Chromosome() = default;

    Chromosome(const std::initializer_list<DblArray<size>>& list) :
        coefficients_{list.begin()[coeffs]},
        standardDeviations_{list.begin()[stddevs]} {}

    Chromosome(const Chromosome& other) :
        coefficients_{other.coefficients_},
        standardDeviations_{other.standardDeviations_} {}

    Chromosome& operator=(const Chromosome& other) {
        if (this != &other) {
            coefficients_ = other.coefficients_;
            standardDeviations_ = other.standardDeviations_;
        }
        return *this;
    }

    Chromosome(Chromosome&& other) noexcept :
        coefficients_{std::move(other.coefficients_)},
        standardDeviations_{std::move(other.standardDeviations_)} {}

    Chromosome& operator=(Chromosome&& other) noexcept {
        if (this != &other) {
            coefficients_ = std::move(other.coefficients_);
            standardDeviations_ = std::move(other.standardDeviations_);
        }
        return *this;
    }

    Chromosome& operator+=(const Chromosome& other) {
        coefficients_ += other.coefficients_;
        standardDeviations_ *= other.standardDeviations_;
        return *this;
    }

    DblArray<size>& operator[](ChromosomeVector vector) {
        switch (vector) {
        case coeffs:
            return coefficients_;
        case stddevs:
            return standardDeviations_;
        }
        throw std::invalid_argument("Nice operator[] bro");
    }

    double operator()(double input) const {
        return coefficients_[a] *
               (input * input -
                coefficients_[b] * std::cos(coefficients_[c] * pi_ * input));
    }

    friend std::ostream& operator<<(std::ostream& stream,
                                    const Chromosome& data) {
        stream << "x: " << data.coefficients_;
        stream << "sigma: " << data.standardDeviations_;
        return stream;
    }

private:
    DblArray<size> coefficients_{};
    DblArray<size> standardDeviations_{};
    inline static const double pi_ = std::acos(-1.0);
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_CHROMOSOME_HPP
