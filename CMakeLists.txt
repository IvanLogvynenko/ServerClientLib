cmake_minimum_required(VERSION 3.16)
project(ServerClientLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

include("references/debugKit.cmake")

include("default_values.cmake")

add_library(ServerClientLib
    src/Server.cpp
    src/Client.cpp
    src/Connection.cpp
)
target_compile_definitions(ServerClientLib PUBLIC EXECUTABLE_NAME="ServerClientLib")
target_link_libraries(ServerClientLib PUBLIC debugKit)
target_include_directories(ServerClientLib PUBLIC include)