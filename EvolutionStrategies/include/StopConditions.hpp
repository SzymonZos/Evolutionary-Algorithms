#ifndef EVOLUTIONSTRATEGIES_STOPCONDITIONS_HPP
#define EVOLUTIONSTRATEGIES_STOPCONDITIONS_HPP

namespace ES {
struct StopConditions {
    double mseThreshold = 0.5;
    std::size_t maxConsecutiveSameMse = 10;
    std::size_t maxIterations = 1000;
    double differenceParentsOffspringMse = 0.0001;
    double mse = 1000.0;
    std::size_t noConsecutiveSameMse = 0;
    std::size_t iteration = 0;
    double currentDiff = 0.0;

    bool notAchieved() {
        return mse >= mseThreshold &&
               noConsecutiveSameMse < maxConsecutiveSameMse &&
               ++iteration < maxIterations &&
               currentDiff >= differenceParentsOffspringMse;
    };
} defaultStop;
} // namespace ES

#endif // EVOLUTIONSTRATEGIES_STOPCONDITIONS_HPP
