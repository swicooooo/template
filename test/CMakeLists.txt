file(GLOB_RECURSE CXX_LIST "*.cpp" test/)

add_executable(mprpc ${CXX_LIST})