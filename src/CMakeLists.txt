file(GLOB_RECURSE CXX_LIST "*.cpp" src/)

add_executable(mprpc ${CXX_LIST})
# add_library(mymuduo SHARED ${SRC_LIST})