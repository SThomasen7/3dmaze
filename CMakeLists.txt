# Minimum version and project setup
cmake_minimum_required(VERSION 3.16)
project(Maze)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-inline")

# Add the renderer directory
add_subdirectory(src)
# add_subdirectory(test)

# Create the main executable
add_executable(maze src/main.cpp)
add_executable(test src/test.cpp)
target_link_libraries(maze PRIVATE MazeLib)
target_link_libraries(test PRIVATE MazeLib MazeTestLib)

# Create the test executable
# add_executable(run_tests test/main.cpp)
# target_link_libraries(run_tests PRIVATE 
# SpectrumRendererLibrary 
# TestHarness 
# TestLibrary 
# CameraTest
# LightTest
# TransformTest
# MeshTest
# ShaderTest
# )

