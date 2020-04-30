cmake_minimum_required(VERSION 3.15)
project(EvolutionaryAlgorithms)

set(CMAKE_CXX_STANDARD 20)

set(SPDLOG_DIR external/spdlog)
set(MATPLOTLIB_DIR external/matplotlib-cpp)
set(MAGIC_ENUM_DIR external/magic_enum)
set(TIMER_DIR external/Timer)
set(OPERATORS_DIR ${PROJECT_SOURCE_DIR}/external/Generic-Ostream-Operators)

set(VER 2)
find_package(Python${VER} COMPONENTS Development NumPy)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        if(NOT MSVC)
            add_compile_definitions(MS_WIN64)
        endif()
    endif()
endif()

add_compile_options(
    -Werror
    -Wall
    -Wextra
    -Wshadow # Warn if variable overshadows parent context
    -Wnon-virtual-dtor # Warn if class with virtual func has no virtual dtor
    -Wcast-align # Warn for potential performance problem casts
    -Wunused # Warn on anything being unused
    -Woverloaded-virtual # Warn if overload a virtual function
    -Wpedantic # Warn if non-standard C++ is used
    -Wconversion # Warn on type conversions that may lose data
    -Wsign-conversion # Warn on sign conversions
    -Wdouble-promotion # Warn if float is implicit promoted to double
    -Wold-style-cast # Warn if c style cast is performed
    -fconstexpr-depth=1024
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -stdlib=libc++
    )
    add_link_options(
        -stdlib=libc++
        -lc++abi
    )
endif()

include_directories(SYSTEM
    ${SPDLOG_DIR}/include
    ${MATPLOTLIB_DIR}
    ${MAGIC_ENUM_DIR}/include
    ${TIMER_DIR}
    ${OPERATORS_DIR}
    ${Python${VER}_INCLUDE_DIRS}
    ${Python${VER}_NumPy_INCLUDE_DIRS}
)

add_subdirectory(${SPDLOG_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${SPDLOG_DIR})

add_subdirectory(TravellingSalesmanProblem)
add_subdirectory(EvolutionStrategies)
