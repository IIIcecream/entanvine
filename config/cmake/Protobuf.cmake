set(PROTOBUF_ROOT ${SYS_THRLIB_DIR}/protobuf)
set(PROTOBUF_INCLUDE_DIR ${PROTOBUF_ROOT}/include)
include_directories(SYSTEM ${PROTOBUF_INCLUDE_DIR})

set(PROTOBUF_LIBRARY_DIR ${PROTOBUF_ROOT}/lib)
link_directories(${PROTOBUF_LIBRARY_DIR})

file(GLOB PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARY_DIR}/libproto*.so)

set(PROTOC_CANDIDATES 
    /usr/bin/protoc
    /usr/local/bin/protoc
)
foreach(PROTOC_CANDIDATE ${PROTOC_CANDIDATES})
    execute_process(
        COMMAND ${PROTOC_CANDIDATE} --version
        OUTPUT_VARIABLE PROTOC_RESULT
    )
    if (NOT ("x${PROTOC_RESULT}" STREQUAL "x"))
        set(PROTOBUF_PROTOC_EXECUTABLE ${PROTOC_CANDIDATE})
        break()
    endif()
endforeach()
if ("x${PROTOBUF_PROTOC_EXECUTABLE}" STREQUAL "x")
    MESSAGE(FATAL_ERROR "protoc not found!!!")
endif()


# copy from FindProtobuf.cmake
function(PROTOBUF_GENERATE_CPP OUTPUT_DIR SRCS HDRS)
    if(NOT ARGN)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE_CPP() called without any proto files")
        return()
    endif()
    if(PROTOBUF_GENERATE_CPP_APPEND_PATH)
        # Create an include path for each file specified
        foreach(FIL ${ARGN})
            get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
            get_filename_component(ABS_PATH ${ABS_FIL} PATH)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    else()
        set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    if(DEFINED PROTOBUF_IMPORT_DIRS)
        foreach(DIR ${PROTOBUF_IMPORT_DIRS})
            get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    endif()
    set(${SRCS})
    set(${HDRS})
    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        list(APPEND ${SRCS} "${OUTPUT_DIR}/${FIL_WE}.pb.cc")
        list(APPEND ${HDRS} "${OUTPUT_DIR}/${FIL_WE}.pb.h")
        add_custom_command(
                OUTPUT "${OUTPUT_DIR}/${FIL_WE}.pb.cc"
                "${OUTPUT_DIR}/${FIL_WE}.pb.h"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}"
                COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
                ARGS --cpp_out  ${OUTPUT_DIR} ${_protobuf_include_path} ${ABS_FIL}
                DEPENDS ${ABS_FIL} ${PROTOBUF_PROTOC_EXECUTABLE}
                COMMENT "Running C++ protocol buffer compiler on ${FIL}"
                VERBATIM )
    endforeach()
    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()
function(PAVARO_PROTOBUF_GENERATE_PYTHON OUTPUT_DIR SRCS)
    if(NOT ARGN)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE_PYTHON() called without any proto files")
        return()
    endif()
    if(PROTOBUF_GENERATE_CPP_APPEND_PATH)
        # Create an include path for each file specified
        foreach(FIL ${ARGN})
            get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
            get_filename_component(ABS_PATH ${ABS_FIL} PATH)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    else()
        set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    if(DEFINED PROTOBUF_IMPORT_DIRS)
        foreach(DIR ${PROTOBUF_IMPORT_DIRS})
            get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    endif()
    set(${SRCS})
    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        list(APPEND ${SRCS} "${OUTPUT_DIR}/${FIL_WE}_pb2.py")
        add_custom_command(
                OUTPUT "${OUTPUT_DIR}/${FIL_WE}_pb2.py"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}"
                COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE} --python_out ${OUTPUT_DIR} ${_protobuf_include_path} ${ABS_FIL}
                DEPENDS ${ABS_FIL} ${PROTOBUF_PROTOC_EXECUTABLE}
                COMMENT "Running Python protocol buffer compiler on ${FIL}"
                VERBATIM )
    endforeach()
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
endfunction()
# By default have PROTOBUF_GENERATE_CPP macro pass -I to protoc
# for each directory where a proto file is referenced.
if(NOT DEFINED PROTOBUF_GENERATE_CPP_APPEND_PATH)
    set(PROTOBUF_GENERATE_CPP_APPEND_PATH TRUE)
endif()

# place where to generate protobuf sources
set(proto_gen_folder ${CMAKE_CURRENT_BINARY_DIR}/../proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../proto)
