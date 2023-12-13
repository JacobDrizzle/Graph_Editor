cmake_minimum_required(VERSION 3.5)  # Updated to a newer version to avoid deprecation warning

# Set the project name
project(GraphEditor)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the SFML and project include directories
include_directories("C:/C++_Librarys/SFML-2.4.2/include")
include_directories("${CMAKE_SOURCE_DIR}/include")

# Add executable
add_executable(GraphEditor main.cpp src/Graph.cpp src/GraphEditor.cpp src/Point.cpp src/Segment.cpp src/utils.cpp)

# Link SFML libraries
target_link_libraries(GraphEditor sfml-graphics sfml-window sfml-system)
