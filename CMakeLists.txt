cmake_minimum_required(VERSION 3.12)
project(HashedGenealogyGraph)

# Set the path to the pybind11Config.cmake file or install directory
set(pybind11_DIR /home/salam/miniconda3/envs/my_env/lib/cmake/pybind11)

# Find pybind11
find_package(pybind11 REQUIRED HINTS ${pybind11_DIR})

# Include directories for the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/HashedGenealogyGraph)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SHA256)
include_directories(${pybind11_INCLUDE_DIRS})

# Add the libraries
add_library(HashedGenealogyGraph STATIC HashedGenealogyGraph/HashedGenealogyGraph.cpp)
add_library(SHA256 STATIC SHA256/SHA256.cpp)

# Link pybind11 modules
target_link_libraries(HashedGenealogyGraph PRIVATE pybind11::module)
target_link_libraries(SHA256 PRIVATE pybind11::module)
