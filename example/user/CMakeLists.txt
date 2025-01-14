file(GLOB_RECURSE CXX_LIST "*.cpp" example/user/)

add_executable(user ${CXX_LIST} ${CMAKE_SOURCE_DIR}/proto/user.pb.cc)

target_link_libraries(user ${CMAKE_SOURCE_DIR}/third/lib/libmprpc.so -lzookeeper_mt protobuf mysqlclient mymuduo pthread)