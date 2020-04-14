#ifndef EVOLUTIONARYALGORITHMS_STOPCONDITIONS_HPP
#define EVOLUTIONARYALGORITHMS_STOPCONDITIONS_HPP

namespace ES {
struct StopConditions {
    double mseThreshold = 0.5;
    std::size_t maxConsecutiveSameMse = 10;
    std::size_t maxIterations = 1000;
    double mse = 1000.0;
    std::size_t noConsecutiveSameMse = 0;
    std::size_t iteration = 0;

    bool notAchieved() {
        return mse >= mseThreshold &&
               noConsecutiveSameMse < maxConsecutiveSameMse &&
               ++iteration < maxIterations;
    };
} defaultStop;
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_STOPCONDITIONS_HPP
