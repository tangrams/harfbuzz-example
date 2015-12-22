cmake_minimum_required(VERSION 2.8.12)
project(harfbuzz-example)

set(EXECUTABLE_NAME harfbuzz-example.out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x -stdlib=libc++")

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw)
target_include_directories(glfw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_link_libraries(glfw INTERFACE ${GLFW_LIBRARIES} -ldl)

# Harfbuzz ICU Freetype
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/harfbuzz-icu-freetype)

# OpenGL
find_package(OpenGL REQUIRED)

file(GLOB SOURCES src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${EXECUTABLE_NAME} freetype harfbuzz icucommon glfw ${OPENGL_LIBRARIES})

# Copy resources
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/fonts/*)
foreach(_resource ${RESOURCES})
    add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        COMMAND POST_BUILD cp -P ${_resource} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()
