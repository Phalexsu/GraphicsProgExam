cmake_minimum_required(VERSION 3.15)
project(Group_3)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/tclap)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/glm)
add_subdirectory(external/stb)
add_subdirectory(application)
add_subdirectory(engine/GLFWApplication)
add_subdirectory(engine/GeometricTools)
add_subdirectory(engine/Rendering)

