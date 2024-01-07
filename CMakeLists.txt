cmake_minimum_required(VERSION 3.16)
project(ServerClientLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

include(default_values.cmake)

include_directories(dependencies/debugKit/include)
include_directories(./src)

add_compile_definitions(DEBUG)
add_compile_definitions(ERROR)
add_compile_definitions(FULL_LOGGING)

add_executable(server
    src/server_main.cpp

    src/Server/Server.cpp

    src/Connection/Connection.cpp
    src/Message/Message.cpp
    src/Responce/Responce.cpp
)
target_compile_definitions(server PRIVATE EXECUTABLE_NAME="SERVER")

add_executable(client
    src/client_main.cpp
    
    src/Client/Client.cpp

    src/Connection/Connection.cpp
    src/Message/Message.cpp
    src/Responce/Responce.cpp
)
target_compile_definitions(client PRIVATE EXECUTABLE_NAME="CLIENT")

add_library(ServerClientLib
    src/Server/Server.cpp
    src/Client/Client.cpp
    src/Connection/Connection.cpp
    src/Message/Message.cpp
    src/Responce/Responce.cpp    
)