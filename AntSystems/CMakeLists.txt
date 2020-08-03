project(AntSystems)

add_executable(AntSystems main.cpp)

target_compile_definitions(AntSystems PRIVATE
    BUILD_INFO="${CMAKE_SYSTEM_NAME} ${TOOLCHAIN_BUILD}"
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

set_target_properties(AntSystems
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${TOOLCHAIN_BUILD}"
)

target_include_directories(AntSystems PRIVATE
    include
)

target_link_libraries(AntSystems PRIVATE
    Python${VER}::Python
    Python${VER}::NumPy
    spdlog::spdlog
)
