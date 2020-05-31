#include "Tasks.hpp"

// Test data
// Cities 1 ((2 + 3 + 0 + 9 + 1 + 9 + 9 + 7) % 5 = 0)
// x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
// y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
//
// Cities 2
// x = {0, 2, 6, 7, 15, 12, 14, 9.5, 7.5, 0.5};
// y = {1, 3, 5, 2.5, -0.5, 3.5, 10, 7.5, 9, 10};
//
// Cities 3
// x = {0, 3, 6, 7, 15, 10, 16, 5, 8, 1.5};
// y = {1, 2, 1, 4.5, -1, 2.5, 11, 6, 9, 12};
//
// Cities 4
// x = {3, 2, 12, 7, 9, 3, 16, 11, 9, 2};
// y = {1, 4, 2, 4.5, 9, 1.5, 11, 8, 10, 7};

int main() {
    auto res = FirstTask<50>({5, 1});
    std::cout << res[0] << std::endl << res[1] << std::endl;

    constexpr std::size_t noAnts = 10;
    constexpr DblArray<noAnts> x = {0, 3, 6, 7, 15, 12, 14, 9, 7, 0};
    constexpr DblArray<noAnts> y = {1, 4, 5, 3, 0, 4, 10, 6, 9, 10};
    constexpr auto distanceMatrix = CalculateDistanceMatrix(x, y);
    auto [result, minCostValue] = SecondTask(distanceMatrix);
    std::cout << result << std::endl << minCostValue;
    PlotSecondTask(x, y, result);
    return 0;
}