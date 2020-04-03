cmake_minimum_required(VERSION 3.15)
project(TravellingSalesmanProblem)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        add_compile_definitions(MS_WIN64)
    endif()
endif()

set(SPDLOG external/spdlog)
set(SPDLOG_DIR ${PROJECT_SOURCE_DIR}/../${SPDLOG})
set(MATPLOTLIB external/matplotlib-cpp)
set(MATPLOTLIB_DIR ${PROJECT_SOURCE_DIR}/../${MATPLOTLIB})

add_executable(TravellingSalesmanProblem main.cpp Timer.cpp)

set_target_properties(TravellingSalesmanProblem
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

set(VER 2)
find_package(Python${VER} COMPONENTS Development NumPy)

target_include_directories(TravellingSalesmanProblem PRIVATE
    include
)

include_directories(SYSTEM
    ${SPDLOG_DIR}/include
    ${MATPLOTLIB_DIR}
    ${Python${VER}_INCLUDE_DIRS}
    ${Python${VER}_NumPy_INCLUDE_DIRS}
)

add_subdirectory(${SPDLOG_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${SPDLOG})

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

target_link_libraries(TravellingSalesmanProblem
    Python${VER}::Python
    Python${VER}::NumPy
    spdlog::spdlog
)
