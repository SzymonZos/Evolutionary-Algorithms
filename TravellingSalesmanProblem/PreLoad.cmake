if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "" FORCE)
endif()
