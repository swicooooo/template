file(GLOB_RECURSE CXX_LIST "*.cpp" "*.cc" src/)

add_executable(qos ${CXX_LIST} ${CMAKE_SOURCE_DIR}/proto/user.pb.cc)
# add_library(mymuduo SHARED ${SRC_LIST})

target_link_libraries(qos ${CMAKE_SOURCE_DIR}/third/lib/libmprpc.so protobuf mysqlclient mymuduo pthread) 