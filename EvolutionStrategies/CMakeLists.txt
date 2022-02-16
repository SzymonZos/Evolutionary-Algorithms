project(EvolutionStrategies)

add_executable(EvolutionStrategies main.cpp src/Model.cpp)

target_compile_definitions(EvolutionStrategies PRIVATE
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

set_target_properties(EvolutionStrategies
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

target_include_directories(EvolutionStrategies PRIVATE
    include
)

target_link_libraries(EvolutionStrategies
    Python${VER}::Python
    Python${VER}::NumPy
    spdlog::spdlog
)
