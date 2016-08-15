cmake_minimum_required(VERSION 2.8.12)
project(harfbuzz-example)

set(EXECUTABLE_NAME harfbuzz-example.out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory(${PROJECT_SOURCE_DIR}/glfw)
include_directories(glfw ${PROJECT_SOURCE_DIR}/glfw/include)
target_link_libraries(glfw INTERFACE ${GLFW_LIBRARIES})

set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "Build glew shared")
add_subdirectory(${PROJECT_SOURCE_DIR}/glew-cmake)
include_directories(${PROJECT_SOURCE_DIR}/glew-cmake/include)

# Harfbuzz ICU Freetype
set(HARFBUZZ_BUILD_ICU ON CACHE BOOL "Enable building of ICU")
set(HARFBUZZ_BUILD_UCDN ON CACHE BOOL "Enable building of UCDN")
add_subdirectory(${PROJECT_SOURCE_DIR}/harfbuzz-icu-freetype)

file(GLOB SOURCES src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${EXECUTABLE_NAME} freetype harfbuzz icucommon glfw libglew_static)

# Copy resources
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/fonts/*)
foreach(_resource ${RESOURCES})
    add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        COMMAND POST_BUILD cp -P ${_resource} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()
