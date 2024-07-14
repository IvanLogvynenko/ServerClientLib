cmake_minimum_required(VERSION 3.16)
project(ServerClientLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/references/")
include("references/debugKit.cmake")

include("default_values.cmake")

if (CMAKE_BUILD_TYPE==DEBUG) 
    add_compile_definitions(LOGGING)
    add_compile_definitions(FULL_LOGGING)
    add_compile_definitions(ERROR)
else()
    if (CMAKE_BUILD_TYPE==RELEASE)
        add_compile_definitions(LOGGING)
    endif()
endif()

add_library(ServerClientLib
    src/Server.cpp
    src/Client.cpp
    src/Connection.cpp
)
target_compile_definitions(ServerClientLib PUBLIC EXECUTABLE_NAME="ServerClientLib")
target_link_libraries(ServerClientLib PUBLIC debugKit)
target_include_directories(ServerClientLib PUBLIC include)