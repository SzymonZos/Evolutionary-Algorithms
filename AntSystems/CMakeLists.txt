project(AntSystems)

add_executable(AntSystems main.cpp)

target_compile_definitions(AntSystems PRIVATE
    BUILD_INFO="${CMAKE_SYSTEM_NAME} ${TOOLCHAIN_BUILD}"
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

set_target_properties(AntSystems
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

target_include_directories(AntSystems PRIVATE
    include
)

target_link_libraries(AntSystems PRIVATE
    Python${VER}::Python
    Python${VER}::NumPy
    spdlog::spdlog
)
