# version
cmake_minimum_required(VERSION 3.1.0)

# project name
project (Entanvine)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config/cmake)

file(GLOB_RECURSE VPTR source/vptr/*.cpp source/vptr/*.h)
source_group(vptr FILES ${VPTR})

add_library(${PROJECT_NAME} ${VPTR})

add_subdirectory("test")

# 头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC "source/")