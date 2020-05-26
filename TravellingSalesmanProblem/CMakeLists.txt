project(TravellingSalesmanProblem)

add_executable(TravellingSalesmanProblem main.cpp)

set_target_properties(TravellingSalesmanProblem
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${TOOLCHAIN_BUILD}"
)

target_include_directories(TravellingSalesmanProblem PRIVATE
    include
)

target_link_libraries(TravellingSalesmanProblem
    Python${VER}::Python
    Python${VER}::NumPy
    spdlog::spdlog
)
