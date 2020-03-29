cmake_minimum_required(VERSION 3.15)
project(TravellingSalesmanProblem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconstexpr-depth=1024")

include_directories(include/)

add_executable(TravellingSalesmanProblem main.cpp Timer.cpp)