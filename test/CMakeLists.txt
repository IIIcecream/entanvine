# version
cmake_minimum_required(VERSION 3.1.0)

# project name
project (test)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE GOOGLETEST googletest/*.cpp googletest/*.cc googletest/*.h)
FILE(GLOB_RECURSE GOOGLEMOCK googlemock/*.cpp googlemock/*.cc googlemock/*.h)
FILE(GLOB TESTCASE testcases/*test.cpp)

source_group(gooletest FILES ${GOOGLETEST})
source_group(goolemock FILES ${GOOGLEMOCK})
source_group(testcases FILES ${TESTCASE})

set(TEST_ALL ${GOOGLETEST} ${GOOGLEMOCK} ${TESTCASE} "main.cpp")

# 指定为可执行文件
add_executable(${PROJECT_NAME} ${TEST_ALL})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "test")
target_link_libraries(${PROJECT_NAME} Entanvine)

# 头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC "googletest/include" "googletest"
                                                  "googlemock/include" "googlemock"
                                                  "../source/")
