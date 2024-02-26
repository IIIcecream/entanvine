# version
cmake_minimum_required(VERSION 3.1.0)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build, which defines the gtest/gmock and gtest_main targets
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_testcase _file)
    if(NOT _file)
        message(FATAL_ERROR "You must provide a '_file''")
    endif(NOT _file)

    if(NOT TARGET tests)
        add_custom_target(tests)
    endif()

    get_filename_component(test_name ${_file} NAME_WE)
    set(_target test-${test_name})

    add_executable(${_target} ${_file})


    target_link_libraries(${_target}
                          Entanvine
                          gtest
                          gmock
                          gtest_main)

    add_test(NAME ${_target} COMMAND $<TARGET_FILE:${_target}>)
    add_dependencies(tests ${_target})
endfunction(add_testcase)

add_testcase(testcases/vptr_test.cpp)
add_testcase(testcases/future_test.cpp)
add_testcase(testcases/cycle_reminder_handler_test.cpp)
