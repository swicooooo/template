file(GLOB_RECURSE CXX_LIST "*.cpp" example/file/)

add_executable(file 
    ${CXX_LIST}
    ${CMAKE_SOURCE_DIR}/src/file/filetransfer.cpp
    ${CMAKE_SOURCE_DIR}/src/db/mysql.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/md5.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/base64.cpp
)

target_link_libraries(file mymuduo mysqlclient pthread)