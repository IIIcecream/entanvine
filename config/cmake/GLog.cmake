set(GLOG_ROOT ${SYS_THRLIB_DIR}/glog)
set(GLOG_INCLUDE_DIR ${GLOG_ROOT}/include)
include_directories(${GLOG_INCLUDE_DIR})

set(GLOG_LIBRARY_DIR ${GLOG_ROOT}/lib)
link_directories(${GLOG_LIBRARY_DIR})

file(GLOB GLOG_LIBRARIES ${GLOG_LIBRARY_DIR}/*.so)
