#include "Timer.hpp"
#include <iostream>

using namespace std::chrono;

Timer::Timer() : startTimePoint_{high_resolution_clock::now()} {}

Timer::~Timer() {
    StopTimer();
}

void Timer::StopTimer() {
    const auto endTimePoint = high_resolution_clock::now();
    const auto start = time_point_cast<milliseconds>
            (startTimePoint_).time_since_epoch().count();
    const auto end = time_point_cast<milliseconds>
            (endTimePoint).time_since_epoch().count();
    std::cout << "Elapsed time: " << end - start << " ms  \n";
}
