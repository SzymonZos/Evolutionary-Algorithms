project(EvolutionStrategies)

add_executable(EvolutionStrategies main.cpp src/Model.cpp)

target_compile_definitions(EvolutionStrategies PRIVATE
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

set_target_properties(EvolutionStrategies
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${TOOLCHAIN_BUILD}"
)

target_include_directories(EvolutionStrategies PRIVATE
    include
)

target_link_libraries(EvolutionStrategies
    Python${VER}::Python
    Python${VER}::NumPy
    spdlog::spdlog
)