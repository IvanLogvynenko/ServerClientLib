cmake_minimum_required(VERSION 3.16)
project(ServerClientLib)

set(SERVER_CLIENT_LIB_VERSION_MAJOR 1)
set(SERVER_CLIENT_LIB_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/references/")
include("references/debugKit.cmake")

include("default_values.cmake")

add_compile_definitions(ERROR)
add_compile_definitions(FULL_LOGGING)

add_library(ServerClientLib
    src/Server.cpp
    src/Client.cpp
    src/Connection.cpp
    src/Message.cpp
    src/Responce.cpp    
)
target_link_libraries(ServerClientLib PUBLIC debugKit)
target_include_directories(ServerClientLib PUBLIC include)
set_target_properties(ServerClientLib PROPERTIES
    VERSION ${SERVER_CLIENT_LIB_VERSION_MAJOR}.${SERVER_CLIENT_LIB_VERSION_MINOR}
    SOVERSION 1
)

install(TARGETS ServerClientLib
    EXPORT ServerClientLibConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/generate_install_manifest.cmake")
install(DIRECTORY include/ DESTINATION include)

