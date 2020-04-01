cmake_minimum_required(VERSION 3.15)
project(TravellingSalesmanProblem)

set(CMAKE_CXX_STANDARD 17)

set(SPDLOG external/spdlog)
set(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../${SPDLOG})
set(MATPLOTLIB external/matplotlib-cpp)
set(MATPLOTLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../${MATPLOTLIB})

add_executable(TravellingSalesmanProblem main.cpp Timer.cpp)

find_package(Python3 COMPONENTS Development NumPy)
set(PYTHON -lpython${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR})

target_include_directories(TravellingSalesmanProblem PUBLIC
    include
    ${SPDLOG_DIR}/include
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
    ${MATPLOTLIB_DIR}
)

target_compile_options(TravellingSalesmanProblem PRIVATE
    -Wall
    -Wextra
    -Wshadow # Warn if variable overshadows parent context
    -Wnon-virtual-dtor # Warn if class with virtual func has novirt dtor
    -Wcast-align # Warn for potential performance problem casts
    -Wunused # Warn on anything being unused
    -Woverloaded-virtual # Warn if overload a virtual function
    -Wpedantic # Warn if non-standard C++ is used
#    -Wconversion # Warn on type conversions that may lose data
#    -Wsign-conversion # Warn on sign conversions
    -Wdouble-promotion # Warn if float is implicit promoted to double
    -fconstexpr-depth=1024
)

target_link_options(TravellingSalesmanProblem PRIVATE
    ${PYTHON}
)

add_subdirectory(${SPDLOG_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${SPDLOG})
