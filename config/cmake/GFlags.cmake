set(GFLAGS_ROOT ${SYS_THRLIB_DIR}/gflags)
set(GFLAGS_INCLUDE_DIR ${GFLAGS_ROOT}/include)
include_directories(${GFLAGS_INCLUDE_DIR})

set(GFLAGS_LIBRARY_DIR ${GFLAGS_ROOT}/lib)
link_directories(${GFLAGS_LIBRARY_DIR})

set(GFLAGS_LIBRARIES ${GFLAGS_LIBRARY_DIR}/libgflags.so)
