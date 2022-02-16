project(TravellingSalesmanProblem)

add_executable(TravellingSalesmanProblem main.cpp)

set_target_properties(TravellingSalesmanProblem
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

target_include_directories(TravellingSalesmanProblem PRIVATE
    include
)

target_link_libraries(TravellingSalesmanProblem
    Python${VER}::Python
    Python${VER}::NumPy
    spdlog::spdlog
)
