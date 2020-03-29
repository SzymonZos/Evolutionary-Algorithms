#ifndef BASICCPPTHREADS_TIMER_HPP
#define BASICCPPTHREADS_TIMER_HPP

#include <chrono>

class Timer {
public:
    Timer();
    ~Timer();

    Timer(const Timer& other) = delete;
    Timer& operator=(const Timer& other) = delete;

    Timer(Timer&& other) = delete;
    Timer& operator=(Timer&& other) = delete;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint_;
    void StopTimer();
};


#endif //BASICCPPTHREADS_TIMER_HPP
