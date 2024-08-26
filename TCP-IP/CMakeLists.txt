cmake_minimum_required(VERSION 3.28)
project(Nothing)

set(CMAKE_CXX_STANDARD 17)

link_libraries(ws2_32.lib)

include_directories(error socket)

add_executable(Nothing
        main.cpp
        socket/socket_util.cpp)