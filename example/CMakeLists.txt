file(GLOB_RECURSE CXX_LIST "*.cpp" example/)

add_executable(mprpc ${CXX_LIST})