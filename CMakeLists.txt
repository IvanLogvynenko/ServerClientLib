cmake_minimum_required(VERSION 3.16)
project(ServerClientLib)

include_directories(dependencies/debugKit/include)

add_compile_definitions(DEBUG)


add_executable(server 
    src/server_main.cpp)

add_executable(client
    src/client_main.cpp)