cmake_minimum_required(VERSION 3.28)
project(Nothing)

set(CMAKE_CXX_STANDARD 17)

link_libraries(ws2_32.lib)

include_directories(error socket)

link_directories(./)

add_executable(tcp
       main.cpp
        socket/socket_util.cpp
)

add_executable(test
        test.cpp
        socket/socket_util.cpp
)

#add_subdirectory(socket)