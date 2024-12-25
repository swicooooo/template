file(GLOB_RECURSE CXX_LIST "*.cpp" "*.cc" src/)

add_executable(qos ${CXX_LIST})
# add_library(mymuduo SHARED ${SRC_LIST})

target_link_libraries(qos ${CMAKE_SOURCE_DIR}/lib/libmprpc.so protobuf mysqlclient mymuduo pthread) 