cmake_minimum_required(VERSION 3.12)
project(HashedGenealogyGraph)

# Set the path to the pybind11Config.cmake file or install directory
set(pybind11_DIR /home/salam/miniconda3/envs/my_env/lib/cmake/pybind11)

# Find pybind11
find_package(pybind11 REQUIRED HINTS ${pybind11_DIR})

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)  # or set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/HashedGenealogyGraph)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SHA256)
include_directories(${pybind11_INCLUDE_DIRS})

# Add the libraries
add_library(HashedGenealogyGraph SHARED HashedGenealogyGraph/HashedGenealogyGraph.cpp)

add_library(SHA256 STATIC SHA256/SHA256.cpp)

# Link pybind11 to the target containing Python bindings
target_link_libraries(HashedGenealogyGraph PRIVATE pybind11::pybind11)
